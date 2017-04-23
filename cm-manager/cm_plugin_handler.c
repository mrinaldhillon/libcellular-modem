#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <dlfcn.h>
#include <link.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_manager_iface.h"
#include "cm_plugin_handler.h"

#define CM_MANAGER_IFACE_NEW_SYM	"cm_manager_iface_new"

typedef struct cm_manager_iface * (*cm_manager_iface_create)(void);

static void * cm_plugin_handler_close_plugin(void *plugin_handle)
{
	char *error = NULL;
	dlclose(plugin_handle);
	if (NULL != (error = dlerror())) {
		cm_warn("Could not close plugin handle %s", error);
	}
	pthread_exit(0);
}

static void cm_plugin_unload(struct cm_manager_iface *cmm_iface,
			     void *plugin_handle)
{
	pthread_t thread_id;
	pthread_create(&thread_id, NULL,
		       cm_plugin_handler_close_plugin, plugin_handle);
	pthread_detach(thread_id);
}

struct cm_manager_iface *
	cm_plugin_handler_load_manager_iface(const char *sopath,
				 int load_in_default_namespace,
				 cm_err_t *err)
{
	char *error = NULL;
	void *plugin_handle = NULL;
	struct cm_manager_iface *cmm_iface = NULL;
	cm_manager_iface_create create_iface = NULL;

	assert(NULL != sopath && NULL != err);

	//@todo:check if sopath exists
	cm_debug("Loading plugin: %s", sopath);
	if (0 == load_in_default_namespace)
		plugin_handle = dlmopen(LM_ID_NEWLM, sopath, RTLD_LAZY);
	else
		plugin_handle = dlopen(sopath, RTLD_LAZY);
	if (NULL == plugin_handle) {
		cm_error("Unable to open plugin handle %s", dlerror());
		*err = CM_ERR_MANAGER_PLUGIN_DLOPEN;
		return NULL;
	}

	create_iface = dlsym(plugin_handle, CM_MANAGER_IFACE_NEW_SYM);
	if (NULL != (error = dlerror())) {
		cm_error("Symbol for function \"%s\" not found %s",
			 CM_MANAGER_IFACE_NEW_SYM, error);
		*err = CM_ERR_MANAGER_PLUGIN_DLSYM;
		goto err_dlsym;
	}

	cmm_iface = create_iface();
	cmm_iface->set_notify_release(cmm_iface,
				      &cm_plugin_unload,
				      plugin_handle);
	return cmm_iface;

err_dlsym:
	dlclose(plugin_handle);
	return NULL;
}

static void cm_plugin_handler_load_plugin(const char *dirpath,
					  const char *filename,
					  int load_in_default_namespace,
					  for_each_loaded loaded,
					  void *userdata,
					  cm_err_t *err)
{
	char *filepath = NULL;
	int filepath_len = 0, dirpath_len = 0;
	struct cm_manager_iface *cmm_iface = NULL;

	assert(NULL != dirpath && NULL != filename
	       && NULL != loaded && NULL != err);
	*err = CM_ERR_NONE;

	dirpath_len = strlen(dirpath);
	// +2 is to compensate for path separartor /.
	filepath_len = dirpath_len + strlen(filename) + 2;

	filepath = (char *)calloc(filepath_len, 1);
	if (NULL == filepath) {
		cm_error("Not enough space %d", errno);
		abort();
	}

	if ('/' == dirpath[dirpath_len-1]) {
		snprintf(filepath, filepath_len, "%s%s",
			 dirpath, filename);
	} else {
		snprintf(filepath, filepath_len, "%s/%s",
			 dirpath, filename);
	}

	//@todo check for filetypes .lo or .so
	cmm_iface = cm_plugin_handler_load_manager_iface(filepath,
						      load_in_default_namespace,
						      err);
	if (NULL == cmm_iface) {
		assert(CM_ERR_NONE != *err);
		cm_warn("Could not load %s, err: %d", filepath, *err);
		goto out_free;
	}

	if (loaded)
		loaded(cmm_iface, userdata);
	cmm_iface->unref(cmm_iface);

out_free:
	if(filepath)
		free(filepath);
}

void cm_plugin_handler_load_from_dir(const char *dirpath,
				     int load_in_default_namespace,
				     for_each_loaded loaded,
				     load_plugin_done done,
				     void *userdata)
{
	DIR *dirp = NULL;
	struct dirent *dirent = NULL;
	cm_err_t err = CM_ERR_NONE;
	int num_loaded = 0;

	assert(NULL != dirpath && (0 != strlen(dirpath)));
	dirp = opendir(dirpath);
	if (NULL == dirp) {
		cm_warn("Unable to open (%s) err: %d", dirpath, errno);
		err = CM_ERR_PLUGIN_HANDLER_OPENDIR;
		goto out_done;
	}

	while(NULL != (dirent = readdir(dirp))) {
		if (0 == strcmp(dirent->d_name, ".") ||
		    0 == strcmp(dirent->d_name, ".."))
			continue;

		cm_plugin_handler_load_plugin(dirpath, dirent->d_name,
					      load_in_default_namespace,
					      loaded, userdata, &err);
		if (CM_ERR_NONE == err)
			num_loaded += 1;
	}

	if (-1 == closedir(dirp))
	    cm_warn("closedir error %d", errno);
	// if no plugins found than this definitely treated as an error.
	if (0 == num_loaded) {
		cm_error("No plugins found at %s", dirpath);
		err = CM_ERR_PLUGIN_HANDLER_ELIBACC;
	}
out_done:
	if (done)
		done(userdata, err);
}
