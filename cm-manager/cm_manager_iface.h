#ifndef _CM_MANAGER_IFACE_H_
#define _CM_MANAGER_IFACE_H_

#include "cm_ref.h"

struct cm_manager_iface;

typedef void (*cm_iface_release_notif)(struct cm_manager_iface *self,
				       void *userdata);

struct cm_manager_iface {
	struct cm_ref refcount;
	//@todo embed cm_list_node
	struct cm_manager_iface_priv *priv;
	struct cm_manager_iface * (*ref)(struct cm_manager_iface *self);
	void (*unref)(struct cm_manager_iface *self);
	void (*set_notify_release)(struct cm_manager_iface *self,
				   cm_iface_release_notif notif,
				   void *userdata);
};

/*
 * Entry point to cm_manager_iface interfaces
 */
struct cm_manager_iface * cm_manager_iface_new(void);

#endif /* _CM_MANAGER_IFACE_H_ */
