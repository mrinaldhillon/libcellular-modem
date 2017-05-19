#ifndef _CM_IP_METHOD_H_
#define _CM_IP_METHOD_H_

typedef enum {
	CM_IP_METHOD_UNKNOWN=0,
	CM_IP_METHOD_PPP,
	CM_IP_METHOD_STATIC,
	CM_IP_METHOD_DHCP
} cm_ip_method_t;

static inline const char * cm_ip_method_to_string(cm_ip_method_t method)
{
	switch(method) {
	case CM_IP_METHOD_UNKNOWN:
		return "UNKNOWN";
	case CM_IP_METHOD_PPP:
		return "PPP";
	case CM_IP_METHOD_STATIC:
		return "STATIC";
	case CM_IP_METHOD_DHCP:
		return "DHCP";
	default:
		return "INVALID METHOD";
	}
}

#endif /* _CM_IP_METHOD_H_ */
