#ifndef _CM_MODEM_CONNECT_PROPERTIES_OBJ_H_
#define _CM_MODEM_CONNECT_PROPERTIES_OBJ_H_

#include "cm_object.h"
#include "cm_err.h"
#include "cm_ip_type.h"

#define CM_MODEM_CONNECT_PROPERTIES_CLASS_NAME		"CMModemConnectProperties"

struct cm_modem_connect_properties_priv;

struct cm_modem_connect_properties {
	struct cm_object cmobj;

	struct cm_modem_connect_properties_priv *priv;

	char * (*get_path)(struct cm_modem_connect_properties *self);

	const char *(*get_class_name)(void);

	struct cm_modem_connect_properties *
		(*get)(struct cm_modem_connect_properties *self);

	void (*put)(struct cm_modem_connect_properties *self);

	const char * (*get_pin)(struct cm_modem_connect_properties *self);

	const char *
		(*get_operator_id)(struct cm_modem_connect_properties *self);

	const char * (*get_apn)(struct cm_modem_connect_properties *self);

	const char * (*get_user)(struct cm_modem_connect_properties *self);

	const char *
		(*get_password)(struct cm_modem_connect_properties *self);

	cm_ip_type_t
		(*get_ip_type)(struct cm_modem_connect_properties *self);

	unsigned int
		(*get_allow_roaming)(struct cm_modem_connect_properties *self);

	const char *
		(*get_number)(struct cm_modem_connect_properties *self);
};

static inline struct cm_modem_connect_properties *
to_cm_modem_connect_properties(struct cm_object *cmobj)
{
	return cmobj ? cm_container_of(cmobj,
				       struct cm_modem_connect_properties,
				       cmobj) : NULL;
}

struct cm_modem_connect_properties *
cm_modem_connect_properties_obj_new(const char *pin,
				     const char *operator_id,
				     const char *apn,
				     const char *user,
				     const char *password,
				     cm_ip_type_t ip_type,
				     unsigned int allow_roaming,
				     const char *number);


#endif /* _CM_MODEM_CONNECT_PROPERTIES_H_ */
