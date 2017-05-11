#ifndef _CM_BEARER_PROPERTIES_OBJ_H_
#define _CM_BEARER_PROPERTIES_OBJ_H_

#include "cm_object.h"
#include "cm_err.h"
#include "cm_ip_type.h"

#define CM_BEARER_PROPERTIES_CLASS_NAME		"CMBearerProperties"

struct cm_bearer_properties_priv;

struct cm_bearer_properties {
	struct cm_object cmobj;

	struct cm_bearer_properties_priv *priv;

	char * (*get_path)(struct cm_bearer_properties *self);

	const char *(*get_class_name)(void);

	struct cm_bearer_properties *
		(*get)(struct cm_bearer_properties *self);

	void (*put)(struct cm_bearer_properties *self);

	const char * (*get_apn)(struct cm_bearer_properties *self);

	const char * (*get_user)(struct cm_bearer_properties *self);

	const char *
		(*get_password)(struct cm_bearer_properties *self);

	cm_ip_type_t
		(*get_ip_type)(struct cm_bearer_properties *self);

	unsigned int
		(*get_allow_roaming)(struct cm_bearer_properties *self);

	const char *
		(*get_number)(struct cm_bearer_properties *self);
};

static inline struct cm_bearer_properties *
to_cm_bearer_properties(struct cm_object *cmobj)
{
	return cmobj ? cm_container_of(cmobj,
				       struct cm_bearer_properties,
				       cmobj) : NULL;
}

struct cm_bearer_properties *
cm_bearer_properties_obj_new(const char *apn,
			 const char *user,
			 const char *password,
			 cm_ip_type_t ip_type,
			 unsigned int allow_roaming,
			 const char *number);


#endif /* _CM_BEARER_PROPERTIES_H_ */
