#ifndef _CM_MANAGER_PRIV_H_
#define _CM_MANAGER_PRIV_H_

#include "cm_atomic.h"
#include "cm_object.h"
#include "cm_module.h"
#include <libmm-glib/libmm-glib.h>

struct cm_manager_priv {
	/* owner is passed when creating cm_manager after
	 * dynamically loading the library i.e. dlopen
	 * cm_manager will take refernce of cm_module
	 * and release it when it gets finalized.
	 */
	struct cm_module *owner;
	struct cm_set *modems;
	cm_atomic_t num_modems;
	MMManager *modem_manager;
	GDBusConnection *dbus_conn;
};

#endif /* _CM_MANAGER_PRIV_H_ */
