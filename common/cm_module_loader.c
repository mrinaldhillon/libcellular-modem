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
#include "cm_module.h"
#include "cm_module_loader.h"

#define CM_MODULE_CREATE_SYM	"get_module_entry_symbol"

typedef const char * (*get_module_entry_symbol)(void);

char * cm_module_loader_build_filepath(const char *dirpath,
				   const char *filename,
				   cm_err_t *err)
{
	char *filepath = NULL;
	int filepath_len = 0, dirpath_len = 0;

	assert(dirpath && dirpath[0] &&
	       filename && filename[0]);

	dirpath_len = strlen(dirpath);
	// +2 is to compensate for path separartor / and last \0.
	filepath_len = dirpath_len + strlen(filename) + 2;

	filepath = (char *)calloc(filepath_len, sizeof(char));
	if (!filepath) {
		cm_error("Unable to allocate enough space %d", errno);
		abort();
	}

	if ('/' == dirpath[dirpath_len-1]) {
		snprintf(filepath, filepath_len, "%s%s",
			 dirpath, filename);
	} else {
		snprintf(filepath, filepath_len, "%s/%s",
			 dirpath, filename);
	}
	return filepath;
}

void cm_module_loader_load_from_dirpath(const char *dirpath,
					int load_in_new_namespace,
					for_each_module_loaded loaded,
					load_modules_done done,
					void *userdata,
					cm_err_t *err)
{
	DIR *dirp = NULL;
	struct dirent *dirent = NULL;
	int num_loaded = 0;
	struct cm_module *module = NULL;
	char *filepath = NULL;

	assert(NULL != dirpath && (0 != strlen(dirpath)) && NULL != err);
	dirp = opendir(dirpath);
	if (NULL == dirp) {
		cm_error("Unable to open (%s) err: %d", dirpath, errno);
		*err |= CM_ERR_MODULE_LOADER_OPENDIR;
		goto out_done;
	}

	while(NULL != (dirent = readdir(dirp))) {
		if (0 == strcmp(dirent->d_name, ".") ||
		    0 == strcmp(dirent->d_name, ".."))
			continue;
		filepath = cm_module_loader_build_filepath(dirpath,
							   dirent->d_name,
							   err);
		// @todo error handling if err is set
		module = cm_module_loader_load_path(filepath,
					   load_in_new_namespace,
					   err);
		free(filepath);
		filepath = NULL;
		if (CM_ERR_NONE != *err) {
			cm_error("Error in loading shared libary name:%s \
				 under directory:%s %d", dirpath,
				 dirent->d_name, *err);
			goto out_closedir;
		}
		if (loaded)
			loaded(module, userdata, err);
		// reduce refcount here since callee of loaded may have
		// taken one, else this module is discarded
		cm_object_put(&module->cmobj);
		if (CM_ERR_NONE != *err) {
			cm_error("Error returned from module_loaded_for_each \
				 callback for libary name:%s from directory:%s %d",
				 dirpath, dirent->d_name, *err);
			break;
		}
		num_loaded += 1;
	}
	// if no plugins found than this definitely treated as an error.
	if (0 == num_loaded) {
		cm_error("No module loaded under directory: %s", dirpath);
		*err |= CM_ERR_MODULE_LOADER_ELIBACC;
	}

out_closedir:
	if (-1 == closedir(dirp)) {
		// @todo pile on one more error here
		cm_warn("closedir error %d", errno);
	}
out_done:
	if (done)
		done(userdata, *err);
}

static void * cm_module_loader_close_handle(void *module_handle)
{
	char *error = NULL;
	dlclose(module_handle);
	if (NULL != (error = dlerror())) {
		cm_warn("Could not close module handle %s", error);
	}
	pthread_exit(0);
}

static void cm_module_loader_unload_module(void *module_handle)
{
	pthread_t thread_id;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&thread_id, &attr,
		       cm_module_loader_close_handle, module_handle);
	pthread_attr_destroy(&attr);
	pthread_detach(thread_id);
}

struct cm_module * cm_module_loader_load_path(const char *filepath,
					      int load_in_new_namespace,
					      cm_err_t *err)
{
	struct cm_module *module = NULL;
	char *error = NULL;
	void *module_handle = NULL;
	get_module_entry_symbol get_entry_symbol = NULL;
	const char *entry_symbol = NULL;

	assert(filepath && filepath[0] && err);

	//@todo:check if sopath exists
	cm_debug("Loading module: %s", filepath);
	if (0 == load_in_new_namespace)
		module_handle = dlmopen(LM_ID_NEWLM, filepath, RTLD_NOW);
	else
		module_handle = dlopen(filepath, RTLD_NOW);
	if (!module_handle) {
		cm_error("Unable to open plugin handle %s", dlerror());
		*err |= CM_ERR_MODULE_LOADER_DLOPEN;
		return NULL;
	}

	get_entry_symbol = dlsym(module_handle, CM_MODULE_CREATE_SYM);
	if (NULL != (error = dlerror())) {
		cm_error("Symbol for module create \"%s\" not found %s",
			 CM_MODULE_CREATE_SYM, error);
		*err |= CM_ERR_MODULE_LOADER_DLSYM;
		goto out_dlclose;
	}
	entry_symbol = get_entry_symbol();
	module = cm_module_create(module_handle, entry_symbol);
	module->release = &cm_module_loader_unload_module;
	return module;

out_dlclose:
	dlclose(module_handle);
	return NULL;
}
