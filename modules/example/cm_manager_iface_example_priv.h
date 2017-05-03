#ifndef _CM_MANAGER_IFACE_EXAMPLE_PRIV_
#define _CM_MANAGER_IFACE_EXAMPLE_PRIV_

typedef void (*notify_release)(struct cm_manager_iface *self, void *userdata);

struct cm_manager_iface_priv {
	notify_release notif;
	void *userdata;
};

#endif /*_CM_MANAGER_IFACE_EXAMPLE_PRIV_*/
