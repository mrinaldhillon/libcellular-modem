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
#define CMM_MANAGER_LIBDIR		"install/usr/lib/cmm-manager"
#endif /* !defined CMM_MANAGER_LIBDIR */
/* @tbd: Should CMMManeger be singleton */


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

static void cmm_manager_release_for_each_cmm(struct cm_object *cmmobj,
					     void *userdata)
{
	assert(cmmobj);
	struct cmm_manager *self = (struct cmm_manager *)userdata;
	struct cm_manager *cmm = to_cm_manager(cmmobj);
	cmm->cleanup(cmm);
	cm_object_del(cmmobj);
	cm_object_put(cmmobj);
	cm_atomic_dec(&self->priv->num_cmm);
}

static void cmm_manager_obj_cleanup(struct cmm_manager *self)
{
	assert(self && self->priv);
	cm_set_for_each_safe(self->priv->cmmset,
			     &cmm_manager_release_for_each_cmm,
			     self);
	assert(0 == cm_atomic_read(&self->priv->num_cmm));
	cm_debug("CMMManager cleanup done");
	cm_set_del(self->priv->cmmset);
	cm_set_put(self->priv->cmmset);
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
	cm_atomic_set(&priv->num_cmm, 0);
	self->priv = priv;

	self->cleanup = &cmm_manager_obj_cleanup;
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
		goto out_unref;
	}
	return self;

out_unref:
	cmm_manager_obj_cleanup(self);
	cm_object_put(&self->cmobj);
	return NULL;
}
