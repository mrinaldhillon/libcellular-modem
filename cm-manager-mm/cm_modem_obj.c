#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "cm_thread.h"
#include "cm_err.h"
#include "cm_log.h"
#include "cm_object.h"
#include "cm_atomic.h"
#include "cm_modem_priv.h"
#include "cm_modem_obj.h"
#include "cm_manager_mm_internal.h"

#define	CMMODEM_CLASS_NAME		"CMModem"

static const char * cm_modem_obj_get_class_name(void)
{
	return CMMODEM_CLASS_NAME;
}

char * cm_modem_obj_get_path(struct cm_modem *self)
{
	assert(self);
	return cm_object_get_path(&self->cmobj);
}

static void cm_modem_obj_for_each_modem_get(struct cm_object *bearerobj,
					      void *userdata)
{
	assert(bearerobj);
	/* @todo: change this cm_modem_get once implemented in modem */
	cm_object_get(bearerobj);
}

static struct cm_modem * cm_modem_obj_get(struct cm_modem *self)
{
	assert(self && self->priv);
	cm_object_get(&self->cmobj);
	cm_set_get(self->priv->bearers);
	cm_set_for_each_safe(self->priv->bearers,
			     &cm_modem_obj_for_each_modem_get,
			     self);

	if (self->priv->mmobj)
		g_object_ref(self->priv->mmobj);
	if (self->priv->mm_modem)
		g_object_ref(self->priv->mm_modem);
	if(self->priv->mm_modem_signal)
		g_object_ref(self->priv->mm_modem_signal);

	return self;
}

static void cm_modem_obj_for_each_modem_put(struct cm_object *bearerobj,
					      void *userdata)
{
	assert(bearerobj);
	/* @todo: change this cm_modem_get once implemented in modem */
	cm_object_get(bearerobj);
}

static void cm_modem_obj_put(struct cm_modem *self)
{
	assert(self && self->priv);
	cm_set_for_each_safe(self->priv->bearers,
			     &cm_modem_obj_for_each_modem_get,
			     self);
	cm_set_put(self->priv->bearers);

	if (self->priv->mmobj)
		g_object_unref(self->priv->mmobj);
	if (self->priv->mm_modem)
		g_object_unref(self->priv->mm_modem);
	if(self->priv->mm_modem_signal)
		g_object_unref(self->priv->mm_modem_signal);

	cm_object_put(&self->cmobj);
}

#if 0
struct cm_modem_obj_put_ctx {
	struct cm_modem *cmm;
	struct cm_set *thread_ctxset;
	cm_err_t err;
};

static void
cm_modem_obj_for_modem_put_thread(struct cm_thread_ctx *thread_ctx)
{
	assert(thread_ctx);
	struct cm_object *bearerobj = (struct cm_object *)thread_ctx->args;
	cm_object_put(bearerobj);

	// balancing ref
	cm_object_put(bearerobj);
}

static void cm_modem_obj_for_each_bearer_put(struct cm_object *bearerobj,
					      void *userdata)
{
	assert(bearerobj);
	cm_err_t err = CM_ERR_NONE;
	struct cm_modem_obj_put_ctx *put_ctx =
		(struct cm_modem_obj_put_ctx *)userdata;
	assert(put_ctx && put_ctx->cmm && put_ctx->thread_ctxset);

	struct cm_thread_ctx *thread_ctx = cm_thread_ctx_new();
	// increase ref of object before pushing into thread ctx
	thread_ctx->args = cm_object_get(bearerobj);
	thread_ctx->start_routine = &cm_modem_obj_for_modem_put_thread;
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
	cm_object_put(bearerobj);
	// actual put serialized since thread creation failed
	cm_object_put(bearerobj);
out_putthread_ctx:
	cm_object_put(&thread_ctx->cmobj);
}

static void cm_modem_obj_put(struct cm_modem *self)
{
	assert(self && self->priv);
	cm_err_t err = CM_ERR_NONE;
	struct cm_modem_obj_put_ctx *put_ctx =
		(struct cm_modem_obj_put_ctx *)calloc(1, sizeof(*put_ctx));
	if (!put_ctx) {
		cm_error("Unable to allocate enough memory %d", errno);
		abort();
	}
	put_ctx->cmm = self;
	put_ctx->thread_ctxset = cm_set_create();
	cm_object_set_name(&put_ctx->thread_ctxset->cmobj, "CMManagerThreads");

	cm_set_for_each_safe(self->priv->bearers,
			     &cm_modem_obj_for_each_bearer_put,
			     put_ctx);

	cm_thread_ctx_join_threads_and_del(put_ctx->thread_ctxset, &err);
	if (CM_ERR_NONE != err) {
		cm_warn("Could not join some/all threads to put bearers, \
			this will result in memory leaks!!! %d", err);
	}

	cm_object_put(&put_ctx->thread_ctxset->cmobj);
	cm_set_put(self->priv->bearers);
	if (self->priv->mmobj)
		g_object_unref(self->priv->mmobj);
	if (self->priv->mm_modem)
		g_object_unref(self->priv->mm_modem);
	if(self->priv->mm_modem_signal)
		g_object_unref(self->priv->mm_modem_signal);
	cm_object_put(&self->cmobj);
	free(put_ctx);
}
#endif

static void cm_modem_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_modem *self = to_cm_modem(cmobj);

	cm_debug("Destroying %s", cm_object_get_name(cmobj));
	free(self->priv);
	free(self);
}

struct cm_modem * cm_modem_obj_new(cm_err_t *err)
{
	struct cm_modem_priv *priv =
		(struct cm_modem_priv *)calloc(1, sizeof(*priv));
	if (NULL == priv) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	struct cm_modem *self =
		(struct cm_modem *)calloc(1, sizeof(*self));
	if (NULL == self) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	cm_object_init(&self->cmobj);
	self->cmobj.release = &cm_modem_obj_release;
	cm_object_set_name(&self->cmobj, CMMODEM_CLASS_NAME);
	priv->bearers = cm_set_create_and_add(&self->cmobj, NULL,
					     err, "CMBearers");
	cm_atomic_set(&priv->num_bearers, 0);
	self->priv = priv;
	self->get = &cm_modem_obj_get;
	self->put = &cm_modem_obj_put;
	self->get_class_name = &cm_modem_obj_get_class_name;
	self->get_path = &cm_modem_obj_get_path;
	return self;
}

//@todo need new and free methods for ctx
struct cm_modem_new_ctx {
	cm_modem_new_done done;
	void *userdata;
};

static void * cm_modem_obj_new_thread(void *userdata)
{
	cm_err_t err = CM_ERR_NONE;
	struct cm_modem *self = NULL;
	struct cm_modem_new_ctx * ctx = NULL;

	ctx = (struct cm_modem_new_ctx *)userdata;
	assert(ctx && ctx->done);

	self = cm_modem_obj_new(&err);
	ctx->done(self, ctx->userdata, err);
	/* remove the reference since callee may have take one
	 * else this object will be released*/
	if (self)
		self->put(self);
	free(ctx);
	return NULL;
}

void cm_modem_obj_new_async(cm_modem_new_done done,
			      void *userdata)
{
	cm_err_t err = CM_ERR_NONE;

	assert(done);
	struct cm_modem_new_ctx *ctx =
		(struct cm_modem_new_ctx *)calloc(1, sizeof(*ctx));
	if (!ctx) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}
	// @todo may need to increase owner's ref
	ctx->done = done;
	ctx->userdata = userdata;

	cm_thread_t thread_id;
	cm_thread_create(&thread_id, &cm_modem_obj_new_thread,
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

void cm_modem_obj_set_mm_modem_object(struct cm_modem *self,
				      MMObject *mm_modemobj,
				      cm_err_t *err)
{
	assert(self && self->priv && mm_modemobj);

	self->priv->mmobj = g_object_ref(mm_modemobj);
	self->priv->mm_modem = mm_object_get_modem(mm_modemobj);
	self->priv->mm_modem_signal = mm_object_get_modem_signal(mm_modemobj);
}
