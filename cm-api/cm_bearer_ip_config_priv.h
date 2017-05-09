#ifndef _CM_BEARER_IP_CONFIG_PRIV_H_
#define _CM_BEARER_IP_CONFIG_PRIV_H_

#include "cm_ip_type.h"

struct cm_bearer_ip_config_priv {
	cm_ip_type ip_type;
	char *address;
	char *prefix;
	char *dns;
	char *gateway;
	char *mtu;
};

#endif /* _CM_BEARER_IP_CONFIG_PRIV_H_ */
