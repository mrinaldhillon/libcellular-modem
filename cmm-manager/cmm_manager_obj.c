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
#include "cmm_manager.h"
#include "cmm_manager_priv.h"
#include "cmm_manager_obj.h"
#include "cm_module.h"
#include "cm_module_loader.h"
#include "cm_modem.h"

#if !defined(CM_PLUGINS_DIR)
#define CM_PLUGINS_DIR		"install/usr/lib/cm-manager"
#endif /* !defined PLUGINS_DIR */
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

static void cmm_manager_obj_cleanup(struct cmm_manager *self)
{
	assert(self && self->priv);
	cm_set_del(self->priv->cmm);
	cm_set_put(self->priv->cmm);
}

static void cmm_manager_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cmm_manager *self = to_cmm_manager(cmobj);
	free(self->priv);
	free(self);
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

	priv->cmm = cm_set_create("CMManagers");
	cm_atomic_set(&priv->num_cmm, 0);
	cm_set_add(priv->cmm, &self->cmobj, NULL, err);
	self->priv = priv;

	self->cleanup = &cmm_manager_obj_cleanup;
	self->start = &cmm_manager_obj_start;
	self->start_async = &cmm_manager_obj_start_async;
	self->stop = &cmm_manager_obj_stop;
	self->stop_async = &cmm_manager_obj_stop_async;
	self->list_modems = &cmm_manager_obj_list_modems;
	self->list_modems_async = &cmm_manager_obj_list_modems_async;

	return self;
}
