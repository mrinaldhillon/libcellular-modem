#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <semaphore.h>
#include "cm_thread.h"
#include "cm_err.h"
#include "cm_log.h"
#include "cm_object.h"
#include "cm_module.h"
#include "cm_manager_priv.h"
#include "cm_manager_obj.h"
#include "cm_modem.h"

#define	CMMANGER_CLASS_NAME		"CMManager"
/* @tbd: Should CMManeger be singleton */
//@todo: revisit unloading logic and semaphore logic
static sem_t mutex;

__attribute__((constructor)) void cm_manager_obj_constructor(void)
{
	sem_init(&mutex, 0, 1);
	cm_debug("Loading: %s", CMMANGER_CLASS_NAME);
}
__attribute__((destructor)) void cm_manager_obj_destructor(void)
{
	sem_wait(&mutex);
	cm_debug("Unloading: %s", CMMANGER_CLASS_NAME);
	sem_destroy(&mutex);
}

static const char * cm_manager_obj_get_class_name(void)
{
	return CMMANGER_CLASS_NAME;
}

static void cm_manager_obj_for_each_modem_get(struct cm_object *modemobj,
					      void *userdata)
{
	assert(modemobj);
	/* @todo: change this cm_modem_get once implemented in modem */
	cm_object_get(modemobj);
}

static struct cm_manager * cm_manager_obj_get(struct cm_manager *self)
{
	assert(self && self->priv);
	cm_object_get(&self->cmobj);
	cm_set_get(self->priv->modems);
	cm_set_for_each_safe(self->priv->modems,
			     &cm_manager_obj_for_each_modem_get,
			     self);
	return self;
}

struct cm_manager_obj_put_ctx {
	struct cm_manager *cmm;
	struct cm_set *thread_ctxset;
	cm_err_t err;
};

static void
cm_manager_obj_for_modem_put_thread(struct cm_thread_ctx *thread_ctx)
{
	assert(thread_ctx);
	struct cm_object *modemobj = (struct cm_object *)thread_ctx->args;
	cm_object_put(modemobj);

	// balancing ref
	cm_object_put(modemobj);
}

static void cm_manager_obj_for_each_modem_put(struct cm_object *modemobj,
					      void *userdata)
{
	assert(modemobj);
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager_obj_put_ctx *put_ctx =
		(struct cm_manager_obj_put_ctx *)userdata;
	assert(put_ctx && put_ctx->cmm && put_ctx->thread_ctxset);

	struct cm_thread_ctx *thread_ctx = cm_thread_ctx_new();
	// increase ref of object before pushing into thread ctx
	thread_ctx->args = cm_object_get(modemobj);
	thread_ctx->start_routine = &cm_manager_obj_for_modem_put_thread;
	cm_thread_ctx_add_and_create_joinable_thread(thread_ctx, NULL,
						     put_ctx->thread_ctxset,
						     &err);
	if (CM_ERR_NONE != err) {
		cm_warn("Could not create joinable thread %d", err);
		goto out_putmodem;
	}
	goto out_putthread_ctx;

out_putmodem:
	cm_warn("Error in put modem object in joinable thread, \
		doing serialized put");
	// balacing ref
	cm_object_put(modemobj);
	// actual put serialized since thread creation failed
	cm_object_put(modemobj);
out_putthread_ctx:
	cm_object_put(&thread_ctx->cmobj);
}

static void cm_manager_obj_put(struct cm_manager *self)
{
	assert(self && self->priv);
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager_obj_put_ctx *put_ctx =
		(struct cm_manager_obj_put_ctx *)calloc(1, sizeof(*put_ctx));
	if (!put_ctx) {
		cm_error("Unable to allocate enough memory %d", errno);
		abort();
	}
	put_ctx->cmm = self;
	put_ctx->thread_ctxset = cm_set_create();
	cm_object_set_name(&put_ctx->thread_ctxset->cmobj, "CMManagerThreads");

	cm_set_for_each_safe(self->priv->modems,
			     &cm_manager_obj_for_each_modem_put,
			     put_ctx);

	cm_thread_ctx_join_threads_and_del(put_ctx->thread_ctxset, &err);
	if (CM_ERR_NONE != err) {
		cm_warn("Could not join some/all threads to put modems, \
			this will result in memory leaks!!! %d", err);
	}

	cm_object_put(&put_ctx->thread_ctxset->cmobj);
	cm_set_put(self->priv->modems);
	cm_object_put(&self->cmobj);
	free(put_ctx);
}

// Start and stop may not be required
static void cm_manager_obj_start(struct cm_manager *self, cm_err_t *err)
{
	assert(self && self->priv && err);
	// start each manager in cmm set
}

static void cm_manager_obj_start_async(struct cm_manager *self,
				       cm_manager_start_done done,
				       void *userdata)
{
	assert(self && self->priv);
	// start each manager in cmm set
}

static void cm_manager_obj_stop(struct cm_manager *self, cm_err_t *err)
{
	assert(self && self->priv && err);
	// stop each manager in cmm set
}

static void cm_manager_obj_stop_async(struct cm_manager *self,
				      cm_manager_stop_done done,
				      void *userdata)
{
	assert(self && self->priv);
	// stop each manager in cmm set
}

static void cm_manager_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_manager *self = to_cm_manager(cmobj);
	struct cm_module *owner = self->priv->owner;

	cm_debug("Destroying %s", cm_object_get_name(cmobj));
	free(self->priv);
	free(self);
	if (owner) {
		/* decrement value to 0 in order to block destructor.
		 * library is unloaded upon relasing owner in release
		 */
		sem_wait(&mutex);
		cm_object_put(&owner->cmobj);
		/* Increment value to allow destructor to proceed
		 * This dirty but required, cannot allow unloading from namespace while
		 * in memory.
		 * @todo figure alternate solution
		 */
		sem_post(&mutex);
	}
}

static struct cm_manager * cm_manager_obj_create(struct cm_module *owner,
						 cm_err_t *err)
{
	assert(err);
	struct cm_manager_priv *priv =
		(struct cm_manager_priv *)calloc(1, sizeof(*priv));
	if (NULL == priv) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	struct cm_manager *self =
		(struct cm_manager *)calloc(1, sizeof(*self));
	if (NULL == self) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	cm_object_init(&self->cmobj);
	self->cmobj.release = &cm_manager_obj_release;
	priv->modems = cm_set_create_and_add(&self->cmobj, NULL,
					     err, "CMModems");
	if (CM_ERR_NONE != *err) {
		cm_error("Error in creating CMModems cmset %d", *err);
		goto out_putself;
	}

	cm_atomic_set(&priv->num_modems, 0);
	self->priv = priv;
	self->get = &cm_manager_obj_get;
	self->put = &cm_manager_obj_put;
	self->start = &cm_manager_obj_start;
	self->start_async = &cm_manager_obj_start_async;
	self->stop = &cm_manager_obj_stop;
	self->stop_async = &cm_manager_obj_stop_async;
	self->list_modems = &cm_manager_obj_list_modems;
	self->list_modems_async = &cm_manager_obj_list_modems_async;
	self->get_class_name = &cm_manager_obj_get_class_name;

	if (owner) {
		cm_object_get(&owner->cmobj);
		priv->owner = owner;
	}

	return self;
out_putself:
	cm_object_put(&self->cmobj);
	return NULL;
}

struct cm_manager * cm_manager_obj_new(struct cm_module *owner,
				       cm_err_t *err)
{
	return cm_manager_obj_create(owner, err);
}
//@todo need new and free methods for ctx
struct cm_manager_new_ctx {
	struct cm_module *owner;
	cm_manager_new_done done;
	void *userdata;
};

static void * cm_manager_obj_new_thread(void *userdata)
{
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager *self = NULL;
	struct cm_manager_new_ctx * ctx = NULL;

	ctx = (struct cm_manager_new_ctx *)userdata;
	assert(ctx && ctx->done);

	self = cm_manager_obj_create(ctx->owner, &err);
	ctx->done(self, ctx->userdata, err);
	/* remove the reference since callee may have take one
	 * else this object will be released*/
	cm_manager_obj_put(self);
	free(ctx);
	return NULL;
}

void cm_manager_obj_new_async(struct cm_module *owner,
			      cm_manager_new_done done,
			      void *userdata)
{
	cm_err_t err = CM_ERR_NONE;

	assert(done);
	struct cm_manager_new_ctx *ctx =
		(struct cm_manager_new_ctx *)calloc(1, sizeof(*ctx));
	if (!ctx) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	ctx->owner = owner;
	ctx->done = done;
	ctx->userdata = userdata;

	cm_thread_t thread_id;
	cm_thread_create(&thread_id, &cm_manager_obj_new_thread,
			 ctx, CM_THREAD_CREATE_DETACHED, &err);
	if (CM_ERR_NONE != err) {
		err |= CM_ERR_MANAGER_NEW;
		goto out_freectx;
	}
	return;
out_freectx:
	free(ctx);
	done(NULL, userdata, err);
}

/* Dynamic library loading is done in two steps
 * 1. symbol = dlsym "cm_manager_obj_get_entry_symbol"
 * 2. struct cm_manager *manager = dlsym symbol
 * @todo 1 step should be sufficient. This is cm_module_loader api issue to keep
 * it independent of specific type to load.
 */
const char * cm_get_module_entry_symbol(void)
{
	return "cm_manager_obj_new";
}
