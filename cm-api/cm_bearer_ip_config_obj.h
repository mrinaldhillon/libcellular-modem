#ifndef _CM_BEARER_IP_CONFIG_OBJ_H_
#define _CM_BEARER_IP_CONFIG_OBJ_H_

#include "cm_ip_type.h"

struct cm_bearer_ip_config_priv;

struct cm_bearer_ip_config {
	struct cm_object cmobj;

	struct cm_bearer_ip_config_priv *priv;

	struct cm_bearer_ip_config *
		(*get)(struct cm_bearer_ip_config *self);

	void (*put)(struct cm_bearer_ip_config *self);

	cm_ip_type_t (*get_ip_type)(struct cm_bearer_ip_config *self);

	const char * (*get_address)(struct cm_bearer_ip_config *self);

	unsigned int (*get_prefix)(struct cm_bearer_ip_config *self);

	const char * (*get_dns)(struct cm_bearer_ip_config *self);

	const char * (*get_gateway)(struct cm_bearer_ip_config *self);

	const char * (*get_mtu)(struct cm_bearer_ip_config *self);
};

struct cm_bearer_ip_config *cm_bearer_ip_config_new(cm_err_t *err);

#endif /* _CM_BEARER_IP_CONFIG_OBJ_H_ */

