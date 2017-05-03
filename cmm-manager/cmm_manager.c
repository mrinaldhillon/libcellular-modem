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

void cmm_manager_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cmm_manager *self = to_cmm_manager(cmobj);
	free(self->priv);
	free(self);
}

struct cmm_manager * cmm_manager_new(cm_err_t *err)
{
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
	self->cmobj.release = &cmm_manager_release;

	priv->cmm = cm_set_create("CMManagers");
	cm_atomic_set(&priv->num_cmm, 0);
	cm_set_add(priv->cmm, &self->cmobj, NULL, err);


	//unload modem manager interfaces
/*	cm_module_loader_load_from_dirpath(CM_PLUGINS_DIR, 1,
					&cmm_manager_for_each_module_loaded,
					&cmm_manager_load_plugin_done,
					self, err);
	if (!CM_ERR_NONE && *err) {
		cm_error("Could not load cm manager plugin interfaces %d",
			 *err);
		goto out_unref;
	}
*/

	return self;
/*out_unref:
	cmm_manager_unref(self);
	return NULL;
*/
}
