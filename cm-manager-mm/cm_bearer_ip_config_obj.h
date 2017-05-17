#ifndef _CM_BEARER_IP_CONFIG_OBJ_H_
#define _CM_BEARER_IP_CONFIG_OBJ_H_

#include "cm_object.h"
#include "cm_ip_method.h"
#include <libmm-glib.h>

#define CM_BEARER_IP_CONFIG_CLASS_NAME		"CMBearerIpConfig"


struct cm_bearer_ip_config {
	struct cm_object cmobj;

	MMBearerIpConfig *mm_ip_config;

	struct cm_bearer_ip_config *
		(*get)(struct cm_bearer_ip_config *self);

	void (*put)(struct cm_bearer_ip_config *self);

	cm_ip_method_t (*get_method)(struct cm_bearer_ip_config *self);

	const char * (*get_address)(struct cm_bearer_ip_config *self);

	unsigned int (*get_prefix)(struct cm_bearer_ip_config *self);

	const char * (*get_dns)(struct cm_bearer_ip_config *self);

	const char * (*get_gateway)(struct cm_bearer_ip_config *self);

	unsigned int (*get_mtu)(struct cm_bearer_ip_config *self);
};

static inline struct cm_bearer_ip_config *
to_cm_bearer_ip_config(struct cm_object *cmobj)
{
	return cmobj ? cm_container_of(cmobj,
				       struct cm_bearer_ip_config,
				       cmobj) : NULL;
}
struct cm_bearer_ip_config *
cm_bearer_ip_config_obj_new(MMBearerIpConfig * mm_ip_config);

#endif /* _CM_BEARER_IP_CONFIG_OBJ_H_ */

