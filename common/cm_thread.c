#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_thread.h"
#include "cm_object.h"

static void cm_thread_ctx_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_thread_ctx *self = cm_container_of(cmobj,
						     struct cm_thread_ctx,
						     cmobj);
	assert(self);
	free(self);
}

struct cm_thread_ctx *cm_thread_ctx_new(void)
{
	struct cm_thread_ctx *self =
		(struct cm_thread_ctx*)calloc(1, sizeof(*self));
	if (!self) {
		cm_error("Unable to allocate enough space %d", errno);
		abort();
	}

	cm_object_init(&self->cmobj);
	self->cmobj.release = &cm_thread_ctx_release;
	return self;
}

void * cm_thread_ctx_thread_routine(void *thread_ctx)
{
	struct cm_thread_ctx *self = (struct cm_thread_ctx *)thread_ctx;
	assert(self && self->start_routine);

	self->start_routine(self);
	cm_object_put(&self->cmobj);
	return NULL;
}

void cm_thread_ctx_create_thread(struct cm_thread_ctx *self,
				 int detachedstate,
				 cm_err_t *err)
{
	assert(self && self->start_routine && err);
	cm_object_get(&self->cmobj);
	cm_thread_create(&self->thread, &cm_thread_ctx_thread_routine,
			 self, detachedstate, err);
	if (CM_ERR_NONE != *err) {
		cm_warn("Error in thread context create %d", *err);
		goto out_putself;
	}
	return;
out_putself:
	cm_object_put(&self->cmobj);
}

void cm_thread_ctx_add_and_create_joinable_thread(struct cm_thread_ctx *self,
						  struct cm_object *parent,
						  struct cm_set *thread_ctxset,
						  cm_err_t *err)
{
	// since object is still being refrenced after adding to set
	cm_object_get(&self->cmobj);
	cm_object_add(&self->cmobj, parent, thread_ctxset, err,
		      "CMThread");
	if (CM_ERR_NONE != *err) {
		cm_warn("Could not add thread ctx to thread set %d", *err);
		goto out_putself;
	}
	cm_thread_ctx_create_thread(self, CM_THREAD_CREATE_JOINABLE, err);
	if (CM_ERR_NONE != *err) {
		cm_warn("Could not create thread deleting thread ctx set%d",
			*err);
		goto out_delself;
	}
	return;
out_delself:
	cm_object_del(&self->cmobj);
out_putself:
	cm_object_put(&self->cmobj);
}

void cm_thread_ctx_join_thread_and_del_for_each(struct cm_object *thread_ctxobj,
						void *privdata)
{
	struct cm_thread_ctx *self = cm_container_of(thread_ctxobj,
						     struct cm_thread_ctx,
						     cmobj);
	cm_err_t *err = (cm_err_t *)privdata;
	cm_thread_join(self->thread, NULL);
	if (CM_ERR_NONE != self->err) {
		cm_warn("Could not join thread: %lu err: %d", self->thread,
			self->err);
		if (!(*err & self->err)) {
			*err |= self->err;
		}
	}
	cm_object_put(&self->cmobj);
}

void cm_thread_ctx_join_threads_and_del(struct cm_set *thread_ctxset,
				       cm_err_t *err)
{
	assert(thread_ctxset);
	cm_set_for_each_safe(thread_ctxset,
			     &cm_thread_ctx_join_thread_and_del_for_each,
			     err);
}


/**
 * This is the wrapper function for pthread_attr_init and pthread_attr_setdetachstate
 * @param attr, variable of type pthread_attr_t
 * @param detachedstate, thread state [PTHREAD_CREATE_JOINABLE or PTHREAD_CREATE_DETACHABLE]
 * @return returns int, 0 on success
 */
static void cm_thread_set_thread_attr(cm_thread_attr_t *attr, int detachedstate,
				      cm_err_t *err)
{
	int ret = CM_ERR_NONE;
	assert(attr);

	ret = pthread_attr_init(attr);
	if (ENOMEM == errno) {
		cm_error("Unable to allocate enough memory %d", errno);
		abort();
	} else if (0 != ret) {
		cm_warn("Could not initialize thread attributes : %d\n", ret);
		*err |= CM_ERR_THREAD_INIT_ATTR;
		return;
	}

	ret = pthread_attr_setdetachstate(attr, detachedstate);
	if (0 != ret) {
		cm_warn("Could not set detach state for thread attributes: \
			%d\n", ret);
		*err |= CM_ERR_THREAD_SET_DETACHED_STATE;
		goto out_attrdestroy;
	}
	return;

out_attrdestroy:
	pthread_attr_destroy(attr);
}

static void cm_thread_destroy_thread_attr(cm_thread_attr_t *attr)
{
	assert(attr);
	pthread_attr_destroy(attr);
}

void cm_thread_create(cm_thread_t *thread,
		      cm_thread_start_routine_t start_routine,
		      cm_thread_arg_t arg, int detachedstate,
		      cm_err_t *err)
{
	cm_thread_attr_t attr;
	int ret = CM_ERR_NONE;

	assert(thread);
	cm_thread_set_thread_attr(&attr, detachedstate, err);
	if (CM_ERR_NONE != *err) {
		cm_warn("Could not set thread attributes : %d\n", *err);
		return;
	}

	ret = pthread_create(thread, &attr, start_routine, arg);
	if (0 != ret) {
		cm_warn("Could not create thread : %d\n", ret);
		*err |= CM_ERR_THREAD_CREATE;
	}

	cm_thread_destroy_thread_attr(&attr);
}
