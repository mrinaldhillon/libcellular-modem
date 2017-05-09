#ifndef _CM_BEARER_PROPERTIES_PRIV_H_
#define _CM_BEARER_PROPERTIES_PRIV_H_

#include "cm_ip_type.h"

struct cm_bearer_properties_priv {
	char *apn;
	char *username;
	char *password;
	cm_ip_type ip_type;
	unsigned int allow_roaming;
	char *number;
	//@todo add more properties here
};

#endif /* _CM_BEARER_PROPERTIES_PRIV_H_ */
