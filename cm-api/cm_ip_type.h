#ifndef _CM_IP_TYPE_H_
#define _CM_IP_TYPE_H_

typedef enum {
	CM_IP_TYPE_NONE=0,
	CM_IP_TYPE_IPV4,
	CM_IP_TYPE_IPV6,
	CM_IP_TYPE_IPV4V6,
	CM_IP_TYPE_ANY
} cm_ip_type_t;

#endif /* _CM_IP_TYPE_H_ */
