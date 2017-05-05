#ifndef _CM_MODEM_IFACE_
#define _CM_MODEM_IFACE_

struct cm_modem {
	struct cm_modem * (*ref)(struct cm_modem *self);
	void (*unref)(struct cm_modem *self);
};

#endif /* _CM_MODEM_IFACE_ */
