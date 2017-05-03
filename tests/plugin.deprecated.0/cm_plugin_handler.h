#ifndef _CM_PLUGIN_HANDLER_
#define _CM_PLUGIN_HANDLER_

#include "cm_err.h"
#include "cm_plugin.h"

typedef void (*for_each_loaded)(struct cm_plugin *plugin,
				void *userdata);
typedef void (*load_plugin_done)(void *userdata,
				cm_err_t err);

void cm_plugin_handler_load_from_dir(const char *dirpath,
				     int load_in_new_namespace,
				     for_each_loaded loaded,
				     load_plugin_done done,
				     void *userdata);

#endif /* _CM_PLUGIN_HANDLER_ */
