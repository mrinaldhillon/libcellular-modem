#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_plugin.h"
#include "cm_plugin_handler.h"

static void cm_plugin_handler_load_plugin(const char *dirpath,
					  const char *filename,
					  int load_in_default_namespace,
					  for_each_loaded loaded,
					  void *userdata,
					  cm_err_t *err)
{
	char *filepath = NULL;
	int filepath_len = 0, dirpath_len = 0;
	struct cm_plugin *plugin = NULL;

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
	plugin = cm_plugin_new(filepath, load_in_default_namespace, err);
	if (CM_ERR_NONE != *err) {
		cm_warn("Could not load %s, err: %d", filepath, *err);
		goto out_free;
	}

	if (loaded)
		loaded(plugin, userdata);
	cm_plugin_unref(plugin);

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
