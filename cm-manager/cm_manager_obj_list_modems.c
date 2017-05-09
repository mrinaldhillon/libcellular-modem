#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "cm_thread.h"
#include "cm_err.h"
#include "cm_log.h"
#include "cm_object.h"
#include "cm_manager_priv.h"
#include "cm_manager_obj.h"
#include "cm_manager_clbk_defs.h"
#include "cm_modem.h"

 //@todo need new and free methods for ctx
struct cm_manager_list_modems_ctx {
	struct cm_manager *self;
	cm_manager_list_modems_for_each for_each;
	void *userdata;
};

static void cm_manager_obj_list_modems_for_each(struct cm_object *modemobj,
						void *userdata)
{
	assert(modemobj);
#if 0
	struct cm_manager *self = NULL;
	struct cm_manager_list_modems_ctx *ctx = NULL;
	struct cm_modem *modem = cm_container_of(modemobj,
						 struct cm_modem, cmobj);

	ctx = (struct cm_manager_list_modems_ctx *)userdata;
	assert(ctx && ctx->self && ctx->for_each);
	ctx->for_each(self, modem, ctx->userdata);
#endif
}

void cm_manager_obj_list_modems(struct cm_manager *self,
			    cm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err)
{
	assert(self && self->priv && for_each && err);

	struct cm_manager_list_modems_ctx *ctx =
		(struct cm_manager_list_modems_ctx *)calloc(1, sizeof(*ctx));
	if (!ctx) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	ctx->self = self;
	ctx->for_each = for_each;
	ctx->userdata = userdata;
	cm_set_for_each_safe(self->priv->modems,
			     &cm_manager_obj_list_modems_for_each,
			     ctx);
	free(ctx);
	return;

}

//@todo need new and free methods for ctx
struct cm_manager_list_modems_async_ctx {
	struct cm_manager *self;
	cm_manager_list_modems_for_each for_each;
	cm_manager_list_modems_done done;
	void *userdata;
};

static void * cm_manager_obj_list_modems_thread(void *userdata)
{
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager *self = NULL;
	struct cm_manager_list_modems_async_ctx *ctx = NULL;

	ctx = (struct cm_manager_list_modems_async_ctx *)userdata;
	assert(ctx && ctx->self && ctx->for_each && ctx->done);
	self = ctx->self;

	cm_manager_obj_list_modems(self, ctx->for_each, ctx->userdata, &err);
	ctx->done(self, ctx->userdata, err);

	free(ctx);
	// balance ref taken before creating thread
	cm_object_put(&self->cmobj);
	return NULL;
}

void cm_manager_obj_list_modems_async(struct cm_manager *self,
				  cm_manager_list_modems_for_each for_each,
				  cm_manager_list_modems_done done,
				  void *userdata)
{
	cm_err_t err = CM_ERR_NONE;

	assert(self && self->priv && for_each && done);
	struct cm_manager_list_modems_async_ctx *ctx =
		(struct cm_manager_list_modems_async_ctx *)
		calloc(1, sizeof(*ctx));

	if (!ctx) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	ctx->self = self;
	ctx->for_each = for_each;
	ctx->done = done;
	ctx->userdata = userdata;
	// increment ref before async call
	cm_object_get(&self->cmobj);

	cm_thread_t thread_id;
	cm_thread_create(&thread_id, &cm_manager_obj_list_modems_thread,
			 ctx, CM_THREAD_CREATE_DETACHED, &err);
	if (CM_ERR_NONE != err) {
		err |= CM_ERR_MANAGER_LIST_MODEMS_ASYNC;
		goto out_freectx;
	}
	return;
out_freectx:
	free(ctx);
	// balance ref
	cm_object_put(&self->cmobj);
	//@todo: this kind of mechanism may confuse developer of client lib
	// consider return error from this function if failure before
	// creating a new thread
	done(self, userdata, err);
}


