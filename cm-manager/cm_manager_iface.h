#ifndef _CM_MANAGER_IFACE_H_
#define _CM_MANAGER_IFACE_H_

#include "cm_list.h"
#include "cm_ref.h"
#include "cm_modem_iface.h"

struct cm_manager_iface;

typedef void (*cm_iface_release_notif)(struct cm_manager_iface *self,
				       void *userdata);
typedef void
(*cm_manager_iface_list_modems_for_each)(struct cm_manager_iface *self,
					 struct cm_modem *modem,
					 void *userdata);

struct cm_manager_iface {
	struct cm_ref refcount;
	struct cm_list_node iface_node;
	struct cm_manager_iface_priv *priv;
	const char * (*get_name)(struct cm_manager_iface *self);
	struct cm_manager_iface * (*ref)(struct cm_manager_iface *self);
	void (*unref)(struct cm_manager_iface *self);
	void (*set_notify_release)(struct cm_manager_iface *self,
				   cm_iface_release_notif notif,
				   void *userdata);
	void (*start)(struct cm_manager_iface *self, cm_err_t *err);
	void (*stop)(struct cm_manager_iface *self, cm_err_t *err);
	void (*list_modems)(struct cm_manager_iface *self,
			    cm_manager_iface_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err);
};

/*
 * Entry point to cm_manager_iface interfaces
 */
struct cm_manager_iface * cm_manager_iface_new(void);

#endif /* _CM_MANAGER_IFACE_H_ */
