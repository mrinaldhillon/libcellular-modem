#ifndef _CM_MODEM_CONNECT_PROPERTIES_H_
#define _CM_MODEM_CONNECT_PROPERTIES_H_

#include "cm_ip_type.h"

struct cm_modem_connect_properties;

struct cm_modem_connect_properties *
cm_modem_connect_properties_new(const char *pin,
				const char *operator_id,
				const char *apn,
				const char *user,
				const char *password,
				cm_ip_type_t ip_type,
				unsigned int allow_roaming,
				const char *number);

// @todo: provide set methods
struct cm_modem_connect_properties *
cm_modem_connect_properties_ref(struct cm_modem_connect_properties *self);

void cm_modem_connect_properties_unref(struct cm_modem_connect_properties *
				       self);

const char *
cm_modem_connect_properties_get_pin(struct cm_modem_connect_properties *self);

const char *
cm_modem_connect_properties_get_operator_id(struct cm_modem_connect_properties
					    *self);

const char *
cm_modem_connect_properties_get_apn(struct cm_modem_connect_properties *self);

const char *
cm_modem_connect_properties_get_user(struct cm_modem_connect_properties *self);

const char *
cm_modem_connect_properties_get_password(struct cm_modem_connect_properties *
					 self);

cm_ip_type_t
cm_modem_connect_properties_get_ip_type(struct cm_modem_connect_properties *
					self);

unsigned int
cm_modem_connect_properties_get_allow_roaming(struct cm_modem_connect_properties
					      *self);

const char *
cm_modem_connect_properties_get_number(struct cm_modem_connect_properties *
				       self);

#endif /* _CM_MODEM_CONNECT_PROPERTIES_H_ */
