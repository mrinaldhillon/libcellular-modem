#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <dlfcn.h>
#include <link.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_container_of.h"
#include "cm_atomic.h"
#include "cm_object.h"
#include "cmm_manager_priv.h"
#include "cmm_manager_obj.h"
#include "cm_module.h"
#include "cm_module_loader.h"
#include "cm_modem.h"
#include "cm_manager_obj.h"

#if !defined(CMM_MANAGER_LIBDIR)
#define CMM_MANAGER_LIBDIR		"/usr/lib/cmm-manager"
#endif /* !defined CMM_MANAGER_LIBDIR */

/* @tbd: Should CMMManeger be singleton */

static void cmm_manager_obj_for_each_cmm_get(struct cm_object *cmmobj,
					     void *userdata)
{
	assert(cmmobj);
	/* @todo: change this cm_modem_get once implemented in modem */
	struct cm_manager *cmm = to_cm_manager(cmmobj);
	cmm->get(cmm);
}

static struct cmm_manager * cmm_manager_obj_get(struct cmm_manager *self)
{
	assert(self && self->priv);
	cm_object_get(&self->cmobj);
	cm_set_get(self->priv->cmmset);
	cm_set_for_each_safe(self->priv->cmmset,
			     &cmm_manager_obj_for_each_cmm_get,
			     self);

	return self;
}

static void cmm_manager_obj_for_each_cmm_put(struct cm_object *cmmobj,
					     void *userdata)
{
	assert(cmmobj);
	/* @todo: change this cm_modem_get once implemented in modem */
	struct cm_manager *cmm = to_cm_manager(cmmobj);
	cmm->put(cmm);
}

static void cmm_manager_obj_put(struct cmm_manager *self)
{
	assert(self && self->priv);
	cm_set_for_each_safe(self->priv->cmmset,
			     &cmm_manager_obj_for_each_cmm_put,
			     self);
	cm_set_put(self->priv->cmmset);
	cm_object_put(&self->cmobj);
}

void cmm_manager_obj_start(struct cmm_manager *self, cm_err_t *err)
{
	assert(self && self->priv && err);
	// start each manager in cmm set
}

void cmm_manager_obj_start_async(struct cmm_manager *self,
			    cmm_manager_start_done done,
			    void *userdata)
{
	assert(self && self->priv);
	// start each manager in cmm set
}

static void cmm_manager_obj_stop(struct cmm_manager *self, cm_err_t *err)
{
	assert(self && self->priv && err);
	// stop each manager in cmm set
}

static void cmm_manager_obj_stop_async(struct cmm_manager *self,
			   cmm_manager_stop_done done,
			   void *userdata)
{
	assert(self && self->priv);
	// stop each manager in cmm set
}

static void cmm_manager_obj_list_modems(struct cmm_manager *self,
			    cmm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err)
{
	assert(self && self->priv && err);
	// list from each manager
}

static void cmm_manager_obj_list_modems_async(struct cmm_manager *self,
				  cmm_manager_list_modems_for_each for_each,
				  cmm_manager_list_modems_done done,
				  void *userdata)
{
	assert(self && self->priv);
	// list from each manager
}

static void cmm_manager_obj_subscribe_modem_added(struct cmm_manager *self,
				      cmm_manager_modem_added added,
				      void *userdata,
				      cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager

}

static void cmm_manager_obj_unsubscribe_modem_added(struct cmm_manager *self,
					cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

static void cmm_manager_obj_subscribe_modem_removed(struct cmm_manager *self,
					cmm_manager_modem_removed removed,
					void *userdata,
					cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

static void cmm_manager_obj_unsubscribe_modem_removed(struct cmm_manager *self,
					  cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

static void cmm_manager_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cmm_manager *self = to_cmm_manager(cmobj);
	cm_debug("Destroying CMMManager");
	free(self->priv);
	free(self);
}

typedef struct cm_manager *
(*cm_manager_create)(struct cm_module *module, cm_err_t *err);

static struct cm_manager *
cmm_manager_obj_load_cmm_from_module(struct cm_module *module, cm_err_t *err)
{
	char *error = NULL;
	cm_manager_create cmm_create = NULL;
	cmm_create = dlsym(module->handle, module->module_entry_symbol);
	if (NULL != (error = dlerror())) {
		cm_error("Symbol to create CMManager object \"%s\" \
			 not found %s", module->module_entry_symbol, error);
		*err |= CM_ERR_MODULE_LOADER_DLSYM;
		return NULL;
	}
	return cmm_create(module, err);
}

static void cmm_manager_obj_for_each_module_loaded(struct cm_module *module,
					     void *userdata,
					     cm_err_t *err)
{
	assert(module);

	int num_cmm = 0;
	struct cmm_manager *self = NULL;
	struct cm_manager *cmm = NULL;

	self = (struct cmm_manager *)userdata;
	assert(self && self->priv);

	num_cmm = cm_atomic_inc_and_read(&self->priv->num_cmm);
	/* add module to under cm manager */
	cm_object_add(&module->cmobj, &self->cmobj, NULL, err,
		      "CMModule-%d", num_cmm);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in adding loadable module object %d", *err);
		goto out_dec_numifaces;
	}

	cmm = cmm_manager_obj_load_cmm_from_module(module, err);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in loading new CMManager object \
			 from module %d", *err);
		goto out_delmodule;
	}

	/* add CMManager to cmmset */
	cm_object_add(&cmm->cmobj, &self->cmobj, self->priv->cmmset, err,
		      "%s-%d", cmm->get_class_name(),
		      cm_atomic_read(&self->priv->num_cmm));
	if (CM_ERR_NONE != *err) {
		cm_error("Error in adding CMManager: %s) \
			 to CMManagers set %d", cmm->get_class_name(), *err);
		goto out_putcmm;
	}

#if defined(CM_DEBUG)
	char *path = NULL;
	path = cm_object_get_path(&cmm->cmobj);
	cm_debug("Path of CMManager %s", path);
	free(path);
#endif
	cm_debug("Loaded CMManager %s", cm_object_get_name(&cmm->cmobj));
	return;
out_putcmm:
	cm_object_put(&cmm->cmobj);
out_delmodule:
	cm_object_del(&module->cmobj);
out_dec_numifaces:
	cm_atomic_dec(&self->priv->num_cmm);
}

static void cmm_manager_obj_load_modules_done(void *userdata,
					cm_err_t err)
{
	struct cmm_manager *self = (struct cmm_manager *)userdata;
	assert(self && self->priv);

	cm_debug("Done loading %d CMManagers with err: %d\n",
		 cm_atomic_read(&self->priv->num_cmm), err);
}

struct cmm_manager * cmm_manager_obj_new(cm_err_t *err)
{
	assert(err);
	struct cmm_manager_priv *priv =
		(struct cmm_manager_priv *)calloc(1, sizeof(*priv));
	if (NULL == priv) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	struct cmm_manager *self =
		(struct cmm_manager *)calloc(1, sizeof(*self));
	if (NULL == self) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	cm_object_init(&self->cmobj);
	cm_object_set_name(&self->cmobj, "CMMManager");
	self->cmobj.release = &cmm_manager_obj_release;

	priv->cmmset = cm_set_create_and_add(&self->cmobj, NULL, err,
					     "CMManagers");
	if (CM_ERR_NONE != *err) {
		cm_error("Error in creating CMManagers set %d", *err);
		goto out_putself;
	}

	cm_atomic_set(&priv->num_cmm, 0);
	self->priv = priv;

	self->get = &cmm_manager_obj_get;
	self->put = &cmm_manager_obj_put;
	self->start = &cmm_manager_obj_start;
	self->start_async = &cmm_manager_obj_start_async;
	self->stop = &cmm_manager_obj_stop;
	self->stop_async = &cmm_manager_obj_stop_async;
	self->list_modems = &cmm_manager_obj_list_modems;
	self->list_modems_async = &cmm_manager_obj_list_modems_async;

	cm_module_loader_load_from_dirpath(CMM_MANAGER_LIBDIR, 1,
					&cmm_manager_obj_for_each_module_loaded,
					&cmm_manager_obj_load_modules_done,
					self, err);
	if (CM_ERR_NONE != *err) {
		cm_error("Could not load cmm manager modules %d", *err);
		goto out_putcmset;
	}
	return self;

out_putcmset:
	cm_set_put(priv->cmmset);
out_putself:
	cm_object_put(&self->cmobj);
	return NULL;
}

#if 0
struct cm_manager_list_for_each_ctx {
	struct cm_manager *self;
	cm_manager_list_modems_for_each for_each;
	void *userdata;
};


void cm_manager_list_modems_for_each_wrapper(struct cm_manager_iface *iface,
					     struct cm_modem *modem,
					     void *userdata)
{
	struct cm_manager *self = NULL;
	struct cm_manager_list_for_each_ctx *ctx = NULL;
	assert(NULL != iface &&
	       NULL != modem &&
	       NULL != userdata);

	ctx = (struct cm_manager_list_for_each_ctx *)userdata;
	assert(NULL != ctx &&
	       NULL != ctx->self &&
	       NULL != ctx->for_each);

	self = ctx->self;
	ctx->for_each(self, modem, userdata);
}

void cm_manager_list_modems(struct cm_manager *self,
			    cm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err)
{
	assert(NULL != self &&
	       NULL != for_each &&
	       NULL != err &&
	       1 != cm_list_empty(&self->ifaces));

	struct cm_manager_iface *iface = NULL, *next = NULL;
	struct cm_manager_list_for_each_ctx ctx;

	memset(&ctx, 0, sizeof(ctx));
	ctx.self = self;
	ctx.for_each = for_each;
	ctx.userdata = userdata;

	cm_list_for_each_safe(&self->ifaces, iface, next, iface_node) {
		//@todo create joinable pthreads for concurrent stop
		//@tbd err for each may have to different
		iface->ref(iface)->
			list_modems(iface,
				    &cm_manager_list_modems_for_each_wrapper,
				    &ctx, err);
		if (CM_ERR_NONE != *err) {
			cm_warn("Error in listing modems from interface name %s err: %d",
				iface->get_name(iface), *err);
		}
		iface->unref(iface);
	}
}
#endif

