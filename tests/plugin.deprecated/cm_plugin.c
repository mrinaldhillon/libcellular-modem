#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <dlfcn.h>
#include <link.h>
#include <errno.h>
#include "cm_ref.h"
#include "cm_err.h"
#include "cm_log.h"
#include "cm_plugin.h"

#define CM_IFACE_NEW_SYM	"cm_iface_new"
typedef struct cm_iface * (*cm_iface_create)(void);

struct cm_plugin {
	struct cm_ref refcount;
	char *sopath;
	void *plugin_handle;
	int namespace_id;
	int load_in_default_namespace;
	cm_iface_create create_iface;
};

static void cm_plugin_load(struct cm_plugin *self,
			   cm_err_t *err)
{
	char *error = NULL;
	void *plugin_handle = NULL;
	cm_iface_create create_iface = NULL;

	//@todo:check if sopath exists
	if (0 == self->load_in_default_namespace)
		plugin_handle = dlmopen(LM_ID_NEWLM, self->sopath, RTLD_NOW);
	else
		plugin_handle = dlopen(self->sopath, RTLD_NOW);
	if (NULL == plugin_handle) {
		cm_error("Unable to open plugin handle %s", dlerror());
		*err = CM_ERR_MANAGER_PLUGIN_DLOPEN;
		return;
	}

	create_iface = dlsym(plugin_handle, CM_IFACE_NEW_SYM);
	if (NULL != (error = dlerror())) {
		cm_error("Symbol for function \"%s\" not found %s",
			 CM_IFACE_NEW_SYM, error);
		*err = CM_ERR_MANAGER_PLUGIN_DLSYM;
		goto err_dlsym;
	}

	self->plugin_handle = plugin_handle;
	self->create_iface = create_iface;
	return;

err_dlsym:
	dlclose(plugin_handle);
	return;
}

static void cm_plugin_unload(struct cm_plugin *self,
			     cm_err_t *err)
{
	char *error = NULL;
	dlclose(self->plugin_handle);
	if (NULL != (error = dlerror())) {
		cm_warn("Could not close plugin handle %s", error);
		*err = CM_ERR_MANAGER_PLUGIN_DLCLOSE;
	}
	self->plugin_handle = NULL;
	self->create_iface = NULL;
}

struct cm_plugin * cm_plugin_new(const char *sopath,
				 int load_in_default_namespace,
				 cm_err_t *err)
{
	assert(NULL != sopath && NULL != err);
	*err = CM_ERR_NONE;

	struct cm_plugin *self = (struct cm_plugin *)malloc(sizeof(*self));
	if (NULL == self) {
		cm_error("Not enough space %d", errno);
		abort();
	}

	self->sopath = strdup(sopath);
	if (NULL == self->sopath) {
		cm_error("Not enough space %d", errno);
		abort();
	}

	self->load_in_default_namespace = load_in_default_namespace;
	cm_plugin_load(self, err);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in loading plugin %d", *err);
		goto err_unload;
	}

	cm_ref_init(&self->refcount);
	return self;
err_unload:
	if (self->sopath)
		free(self->sopath);
	free(self);
	return NULL;
}

struct cm_plugin * cm_plugin_ref(struct cm_plugin *self)
{
	assert(NULL != self);
	cm_ref_get(&self->refcount);
	return self;
}

void cm_plugin_unref(struct cm_plugin *self)
{
	cm_err_t err = CM_ERR_NONE;
	assert(NULL != self);
	if (0 == cm_ref_put_is_final(&self->refcount))
		return;

	cm_plugin_unload(self, &err);
	if (CM_ERR_NONE != err)
		cm_warn("Could not unload the plugin %s err: %d",
			self->sopath, err);

	free(self->sopath);
	free(self);
}

struct cm_iface * cm_plugin_create_iface(struct cm_plugin *self)
{
	assert(NULL != self);
	return self->create_iface();
}
