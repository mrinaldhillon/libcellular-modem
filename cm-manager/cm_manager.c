#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_ref.h"
#include "cm_container_of.h"
#include "cm_manager.h"
#include "cm_manager_private.h"
#include "cm_plugin_handler.h"

#if !defined(CM_PLUGINS_DIR)
#define CM_PLUGINS_DIR		"install/usr/lib/cm-manager"
#endif /* !defined PLUGINS_DIR */

static void cm_manager_for_each_iface_loaded(struct cm_manager_iface *iface,
					     void *userdata)
{
	assert(NULL != iface);

	//@todo add a unique id in cm_manager for sanity check
	struct cm_manager *self = (struct cm_manager *)userdata;
	struct cm_manager_iface * iface_entry = iface->ref(iface);
	cm_list_add(&self->ifaces, &iface_entry->iface_node);
	cm_debug("Loaded manager iface %s", iface->get_name(iface));
	self->num_ifaces++;
}

static void cm_manager_load_plugin_done(void *userdata,
			     cm_err_t err)
{
	struct cm_manager *self = (struct cm_manager *)userdata;
	cm_debug("Done loading %u cm manager ifaces with err: %d\n",
		 self->num_ifaces, err);
}

static void cm_manager_release(struct cm_ref *ref)
{
	assert(NULL != ref);
	struct cm_manager *self =
		cm_container_of(ref, struct cm_manager, refcount);
	struct cm_manager_iface *iface = NULL, *next = NULL;

	//unload modem manager interfaces
	cm_list_for_each_safe(&self->ifaces, iface, next, iface_node) {
		cm_list_del(&iface->iface_node);
		iface->unref(iface);
	}

	cm_debug("destroying cm_manager");
	free(self);
}

struct cm_manager * cm_manager_new(cm_err_t *err)
{
	struct cm_manager *self =
		(struct cm_manager *)calloc(sizeof(*self), 1);
	if (NULL == self) {
		cm_error("Not enough space %d",errno);
		abort();
	}

	cm_list_head_init(&self->ifaces);
	//unload modem manager interfaces
	cm_plugin_handler_load_from_dir(CM_PLUGINS_DIR, 1,
					&cm_manager_for_each_iface_loaded,
					&cm_manager_load_plugin_done,
					self, err);
	if (CM_ERR_NONE != *err) {
		cm_error("Could not load cm manager plugin interfaces %d",
			 *err);
		goto out_free;
	}

	cm_ref_init(&self->refcount);
	return self;
out_free:
	free(self);
	return NULL;
}

struct cm_manager * cm_manager_ref(struct cm_manager *self)
{
	assert(NULL != self);
	//@tbd: how to handle ref count of ifaces
	cm_ref_get(&self->refcount);
	return self;
}

void cm_manager_unref(struct cm_manager *self)
{
	assert(NULL != self);
	cm_ref_put(&self->refcount, &cm_manager_release);
}


#if 0
not required, such api may add complexity
void cm_manager_start(struct cm_manager *self,
		      cm_err_t *err)
{
	assert(NULL != self);
	//load modem manager interface into local list.
	//start on each
}

void cm_manager_stop(struct cm_manager *self,
		     cm_err_t *err)
{
	assert(NULL != self);
	//stop on each manager
	//unload modules
}
cm_list * cm_manager_list_modems(struct cm_manager *self,
				 cm_err_t *err)
{
	assert(NULL != self);
	//fetch list from each manager
	//append into a single list
	//destroy the list from each modem
	//return list
}
#endif
