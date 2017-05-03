#ifndef _CM_IFACE_H_
#define _CM_IFACE_H_

struct cm_iface {
	struct cm_iface_priv *priv;
	struct cm_iface * (*ref)(struct cm_iface *self);
	void (*unref)(struct cm_iface *self);
};

/*
 * Entry point to cm_iface interfaces
 */
struct cm_iface * cm_iface_new(void);

#endif /* _CM_IFACE_H_ */
