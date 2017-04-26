#ifndef _CM_PLUGIN_H_
#define _CM_PLUGIN_H_

#include "cm_iface.h"

struct cm_plugin;

struct cm_plugin * cm_plugin_new(const char *sopath,
				 int load_in_default_namespace,
				 cm_err_t *err);
struct cm_plugin * cm_plugin_ref(struct cm_plugin *plugin);
void cm_plugin_unref(struct cm_plugin *plugin);
struct cm_iface *
	cm_plugin_create_iface(struct cm_plugin *self);

#endif /* _CM_PLUGIN_H_ */


