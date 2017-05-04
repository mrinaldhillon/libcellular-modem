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
#include "cm_manager.h"
#include "cm_manager_priv.h"
#include "cm_manager_obj.h"
#include "cm_module.h"
#include "cm_module_loader.h"
#include "cm_modem.h"

#if !defined(CM_PLUGINS_DIR)
#define CM_PLUGINS_DIR		"install/usr/lib/cm-manager"
#endif /* !defined PLUGINS_DIR */
/* @tbd: Should CMMManeger be singleton */



void cm_manager_obj_start(struct cm_manager *self, cm_err_t *err)
{
	assert(self && self->priv && err);
	// start each manager in cmm set
}

void cm_manager_obj_start_async(struct cm_manager *self,
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

static void cm_manager_obj_list_modems(struct cm_manager *self,
			    cm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err)
{
	assert(self && self->priv && err);
	// list from each manager
}

static void cm_manager_obj_list_modems_async(struct cm_manager *self,
				  cm_manager_list_modems_for_each for_each,
				  cm_manager_list_modems_done done,
				  void *userdata)
{
	assert(self && self->priv);
	// list from each manager
}

static void cm_manager_obj_subscribe_modem_added(struct cm_manager *self,
				      cm_manager_modem_added added,
				      void *userdata,
				      cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager

}

static void cm_manager_obj_unsubscribe_modem_added(struct cm_manager *self,
					cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

static void cm_manager_obj_subscribe_modem_removed(struct cm_manager *self,
					cm_manager_modem_removed removed,
					void *userdata,
					cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

static void cm_manager_obj_unsubscribe_modem_removed(struct cm_manager *self,
					  cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

static void cm_manager_obj_cleanup(struct cm_manager *self)
{
	assert(self && self->priv);
	cm_set_del(self->priv->modems);
	cm_set_put(self->priv->modems);
}

static void cm_manager_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_manager *self = to_cm_manager(cmobj);
	free(self->priv);
	free(self);
}

struct cm_manager * cm_manager_obj_new(cm_err_t *err)
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
	cm_object_set_name(&self->cmobj, "CMMManager");
	self->cmobj.release = &cm_manager_obj_release;

	priv->modems = cm_set_create("CMManagers");
	cm_atomic_set(&priv->num_modems, 0);
	cm_set_add(priv->modems, &self->cmobj, NULL, err);
	self->priv = priv;

	self->cleanup = &cm_manager_obj_cleanup;
	self->start = &cm_manager_obj_start;
	self->start_async = &cm_manager_obj_start_async;
	self->stop = &cm_manager_obj_stop;
	self->stop_async = &cm_manager_obj_stop_async;
	self->list_modems = &cm_manager_obj_list_modems;
	self->list_modems_async = &cm_manager_obj_list_modems_async;

	return self;
}
