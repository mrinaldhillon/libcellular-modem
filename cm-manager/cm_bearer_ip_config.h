#ifndef _CM_BEARER_IP_CONFIG_H_
#define _CM_BEARER_IP_CONFIG_H_

struct cm_bearer_ip_confi;

struct cm_bearer_ip_config *
cm_bearer_ip_config_ref(struct cm_bearer_ip_config *self);

void cm_bearer_ip_config_unref(struct cm_bearer_ip_config *self);

#endif /* _CM_BEARER_IP_CONFIG_H_ */
