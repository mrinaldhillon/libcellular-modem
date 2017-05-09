#ifndef _CM_BEARER_PROPERTIES_H_
#define _CM_BEARER_PROPERTIES_H_

#include "cm_ip_type.h"

struct cm_bearer_properties;

struct cm_bearer_properties *
cm_bearer_properties_new(const char *apn,
			 const char *user,
			 const char *password,
			 cm_ip_type_t ip_type,
			 unsigned int allow_roaming,
			 const char *number);

// @todo: provide set methods
struct cm_bearer_properties *
cm_bearer_properties_ref(struct cm_bearer_properties *self);

void cm_bearer_properties_unref(struct cm_bearer_properties *self);

const char * cm_bearer_properties_get_apn(struct cm_bearer_properties *self);

const char * cm_bearer_properties_get_user(struct cm_bearer_properties *self);

const char *
cm_bearer_properties_get_password(struct cm_bearer_properties *self);

cm_ip_type_t
cm_bearer_properties_get_ip_type(struct cm_bearer_properties *self);

unsigned int
cm_bearer_properties_get_allow_roaming(struct cm_bearer_properties *self);

const char *
cm_bearer_properties_get_number(struct cm_bearer_properties *self);

#endif /* _CM_BEARER_PROPERTIES_H_ */
