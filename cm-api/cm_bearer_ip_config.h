#ifndef _CM_BEARER_IP_CONFIG_H_
#define _CM_BEARER_IP_CONFIG_H_

#include "cm_ip_method.h"
/*
 * Ip config is not live object, hence once accessed discard it and
 * fetch the object from cm_bearer for subsequent accesses.
 * Unref it to destroy
 * const string pointers will become dangling pointers once the object is
 * destroyed.
 */
struct cm_bearer_ip_config;

struct cm_bearer_ip_config *
cm_bearer_ip_config_ref(struct cm_bearer_ip_config *self);

void cm_bearer_ip_config_unref(struct cm_bearer_ip_config *self);

cm_ip_method_t cm_bearer_ip_config_get_method(struct cm_bearer_ip_config *self);

const char * cm_bearer_ip_config_get_address(struct cm_bearer_ip_config *self);

unsigned int cm_bearer_ip_config_get_prefix(struct cm_bearer_ip_config *self);

const char ** cm_bearer_ip_config_get_dns(struct cm_bearer_ip_config *self);

const char * cm_bearer_ip_config_get_gateway(struct cm_bearer_ip_config *self);

unsigned int cm_bearer_ip_config_get_mtu(struct cm_bearer_ip_config *self);

#endif /* _CM_BEARER_IP_CONFIG_H_ */
