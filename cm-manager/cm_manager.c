#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <dlfcn.h>
#include <link.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_ref.h"
#include "cm_list.h"
#include "cm_container_of.h"
#include "cm_manager.h"
#include "cm_manager_private.h"
#include "cm_module.h"
#include "cm_module_loader.h"
#include "cm_modem.h"
#include "cm_atomic.h"
#include "cm_object.h"

#if !defined(CM_PLUGINS_DIR)
#define CM_PLUGINS_DIR		"install/usr/lib/cm-manager"
#endif /* !defined PLUGINS_DIR */

/*struct cm_manager_ifaces_list {
	struct cm_list_head head;
	int num_nodes;
};

struct cm_manager_iface_list_entry {
	struct cm_manager_iface *iface;
	struct cm_list_node node;
};*/

typedef struct cm_manager_iface *
(*cm_manager_iface_create)(struct cm_module *module);

static struct cm_manager_iface *
cm_manager_load_iface_from_module(struct cm_module *module,
				  cm_err_t *err)
{
	char *error = NULL;
	struct cm_manager_iface *iface = NULL;
	cm_manager_iface_create iface_create = NULL;
	iface_create = dlsym(module->handle, module->module_entry_symbol);
	if (NULL != (error = dlerror())) {
		cm_error("Symbol for cm_manager iface create \"%s\" not found %s",
			 module->module_entry_symbol, error);
		*err |= CM_ERR_MODULE_LOADER_DLSYM;
		return NULL;
	}
	iface = iface_create(module);
	return iface;
}

static void cm_manager_for_each_module_loaded(struct cm_module *module,
					     void *userdata,
					     cm_err_t *err)
{
	int num_ifaces = 0;
	struct cm_manager *self = NULL;
	struct cm_manager_iface *iface = NULL;

	assert(module);

	self = (struct cm_manager *)userdata;
	num_ifaces = cm_atomic_inc_and_read(&self->num_ifaces);
	/* add module to under cm manager */
	cm_object_add(&module->cmobj, &self->cmobj, NULL, err,
		      "CMModule-%d", num_ifaces);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in adding cmmanager iface(class name: %s) \
			 to ifaces set %d", iface->get_name(iface), *err);
		goto out_dec_numifaces;
	}

	iface = cm_manager_load_iface_from_module(module, err);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in loading new cmmanager interface \
			 from module %d", *err);
		goto out_delmodule;
	}

	/* add iface under cm manager */
	cm_object_add(&iface->cmobj, &self->cmobj, self->ifaces, err,
		      "%s-%d", iface->get_name(iface),
		      cm_atomic_read(&self->num_ifaces));
	if (CM_ERR_NONE != *err) {
		cm_error("Error in adding cmmanager iface(class name: %s) \
			 to ifaces set %d", iface->get_name(iface), *err);
		goto out_putiface;
	}

#if defined(CM_DEBUG)
	char *path = NULL;
	path = cm_object_get_path(&iface->cmobj);
	cm_debug("Path of iface %s", path);
	free(path);
#endif
	cm_debug("Loaded manager iface %s", cm_object_get_name(&iface->cmobj));
	return;
out_putiface:
	cm_object_put(&iface->cmobj);
out_delmodule:
	cm_object_del(&module->cmobj);
out_dec_numifaces:
	cm_atomic_dec(&self->num_ifaces);
}

static void cm_manager_load_plugin_done(void *userdata,
					cm_err_t err)
{
	struct cm_manager *self = (struct cm_manager *)userdata;
	cm_debug("Done loading %d cm manager ifaces with err: %d\n",
		 cm_atomic_read(&self->num_ifaces), err);
}

static void cm_manager_release_for_each_iface(struct cm_object *ifaceobj,
					      void *userdata)
{
	assert(ifaceobj);
	struct cm_manager *self = (struct cm_manager *)userdata;
	cm_object_del(ifaceobj);
	cm_object_put(ifaceobj);

	cm_atomic_dec(&self->num_ifaces);
}

static void cm_manager_release(struct cm_object *cmobj)
{
	assert(NULL != cmobj);
	struct cm_manager *self =
		cm_container_of(cmobj, struct cm_manager, cmobj);

	cm_debug("destroying cm_manager");
	free(self);
}

static void cm_manager_cleanup(struct cm_manager *self)
{
	cm_set_for_each_safe(self->ifaces, &cm_manager_release_for_each_iface,
			     self);
	if (0 != cm_atomic_read(&self->num_ifaces)) {
		cm_warn("Could not unload all ifaces");
	}
	cm_set_del(self->ifaces);
	cm_set_put(self->ifaces);
}

struct cm_manager * cm_manager_new(cm_err_t *err)
{
	struct cm_manager *self =
		(struct cm_manager *)calloc(1, sizeof(*self));
	if (NULL == self) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	cm_object_init(&self->cmobj);
	cm_object_set_name(&self->cmobj, "CMManager");
	self->cmobj.release = &cm_manager_release;
	self->ifaces = cm_set_create("CMManagerIfaces");
	cm_atomic_set(&self->num_ifaces, 0);
	cm_set_add(self->ifaces, &self->cmobj, NULL, err);
	//unload modem manager interfaces
	cm_module_loader_load_from_dirpath(CM_PLUGINS_DIR, 1,
					&cm_manager_for_each_module_loaded,
					&cm_manager_load_plugin_done,
					self, err);
	if (!CM_ERR_NONE && *err) {
		cm_error("Could not load cm manager plugin interfaces %d",
			 *err);
		goto out_unref;
	}

	return self;
out_unref:
	cm_manager_unref(self);
	return NULL;
}

struct cm_manager * cm_manager_ref(struct cm_manager *self)
{
	assert(NULL != self);
	//@tbd: how to handle ref count of ifaces
	cm_object_get(&self->cmobj);
	return self;
}

void cm_manager_unref(struct cm_manager *self)
{
	assert(NULL != self);
	cm_object_put(&self->cmobj);
}

void cm_manager_destroy(struct cm_manager *self)
{
	assert(self);
	cm_manager_cleanup(self);
	cm_object_put(&self->cmobj);
}

static void cm_manager_start_iface(struct cm_object *ifaceobj,
					     void *userdata)
{
	cm_err_t err = CM_ERR_NONE;

	struct cm_manager_iface *iface = to_cm_manager_iface(ifaceobj);
	/* Get and put are only required in async version of this api */
	cm_object_get(ifaceobj);

	iface->start(iface, &err);
	if (CM_ERR_NONE != err) {
		cm_warn("Could not start manager interface name %s err: %d",
			cm_object_get_name(&iface->cmobj), err);
	}
	cm_object_put(ifaceobj);
}

void cm_manager_start(struct cm_manager *self,
		      cm_err_t *err)
{
	assert(self && err && 0 < cm_atomic_read(&self->num_ifaces));
	cm_set_for_each(self->ifaces, &cm_manager_start_iface, self);
}

static void cm_manager_stop_iface(struct cm_object *ifaceobj,
					     void *userdata)
{
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager_iface *iface = to_cm_manager_iface(ifaceobj);
	cm_object_get(ifaceobj);

	iface->stop(iface, &err);
	if (CM_ERR_NONE != err) {
		cm_warn("Could not stop manager interface name %s err: %d",
			cm_object_get_name(&iface->cmobj), err);
	}
	cm_object_put(ifaceobj);
}

void cm_manager_stop(struct cm_manager *self,
		     cm_err_t *err)
{
	assert(self && err && 0 < cm_atomic_read(&self->num_ifaces));
	cm_set_for_each(self->ifaces, &cm_manager_stop_iface, self);

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

