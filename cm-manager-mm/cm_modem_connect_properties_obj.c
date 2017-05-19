#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <libmm-glib.h>
#include "cm_log.h"
#include "cm_err.h"
#include "cm_object.h"
#include "cm_ip_type.h"
#include "cm_modem_connect_properties_obj.h"
#include "cm_modem_connect_properties_priv.h"

static const char *
cm_modem_connect_properties_obj_get_pin(struct
					cm_modem_connect_properties *self)
{
	return (const char *)
		mm_simple_connect_properties_get_pin(self->priv->
						     mm_simple_connect_properties);
}

static const char *
cm_modem_connect_properties_obj_get_operator_id(struct
						cm_modem_connect_properties *self)
{
	return (const char *)
		mm_simple_connect_properties_get_operator_id(self->priv->
							     mm_simple_connect_properties);
}

static const char *
cm_modem_connect_properties_obj_get_apn(struct
					cm_modem_connect_properties *self)
{
	return (const char *)
		mm_simple_connect_properties_get_apn(self->priv->
						     mm_simple_connect_properties);
}

static const char *
cm_modem_connect_properties_obj_get_user(struct
					 cm_modem_connect_properties *self)
{
	return (const char *)
		mm_simple_connect_properties_get_user(self->priv->
						      mm_simple_connect_properties);
}

static const char *
cm_modem_connect_properties_obj_get_password(struct
					     cm_modem_connect_properties *self)
{
	return (const char *)
		mm_simple_connect_properties_get_password(self->priv->
							  mm_simple_connect_properties);
}

static unsigned int
cm_modem_connect_properties_obj_get_allow_roaming(struct
						  cm_modem_connect_properties *self)
{
	return (int)
		mm_simple_connect_properties_get_allow_roaming(self->priv->
							       mm_simple_connect_properties);
}

static const char *
cm_modem_connect_properties_obj_get_number(struct cm_modem_connect_properties *self)
{
	return (const char *)
		mm_simple_connect_properties_get_number(self->priv->
							mm_simple_connect_properties);
}

static cm_ip_type_t
cm_modem_connect_properties_obj_get_ip_type(struct cm_modem_connect_properties *self)
{
	MMBearerIpFamily ip_type =
		mm_simple_connect_properties_get_ip_type(self->priv->
							 mm_simple_connect_properties);
	switch(ip_type) {
	case MM_BEARER_IP_FAMILY_NONE:
		return CM_IP_TYPE_NONE;
	case MM_BEARER_IP_FAMILY_IPV4:
		return CM_IP_TYPE_IPV4;
	case MM_BEARER_IP_FAMILY_IPV6:
		return CM_IP_TYPE_IPV6;
	case MM_BEARER_IP_FAMILY_IPV4V6:
		return CM_IP_TYPE_IPV4V6;
	case MM_BEARER_IP_FAMILY_ANY:
		return CM_IP_TYPE_ANY;
	default:
		cm_warn("Invalid MMBearerIpFamily check ModemManager doc");
		return CM_IP_TYPE_ANY;
	}
}

char * cm_modem_connect_properties_obj_get_path(struct cm_modem_connect_properties *self)
{
	assert(self);
	return cm_object_get_path(&self->cmobj);
}

static struct cm_modem_connect_properties *
cm_modem_connect_properties_obj_get(struct cm_modem_connect_properties *self)
{
	assert(self && self->priv);
	cm_object_get(&self->cmobj);

	return self;
}

static void cm_modem_connect_properties_obj_put(struct cm_modem_connect_properties *self)
{
	assert(self && self->priv);
	cm_object_put(&self->cmobj);
}

static void cm_modem_connect_properties_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_modem_connect_properties *self = to_cm_modem_connect_properties(cmobj);

	cm_debug("Destroying %s", cm_object_get_name(cmobj));
	if (self->priv->mm_simple_connect_properties)
		g_object_unref(self->priv->mm_simple_connect_properties);

	free(self->priv);
	free(self);
}

static MMSimpleConnectProperties *
cm_modem_connect_properties_obj_set_mm_simple_connect_properties(const char *pin,
								 const char *operator_id,
								 const char *apn,
								 const char *user,
								 const char *password,
								 cm_ip_type_t ip_type,
								 unsigned int allow_roaming,
								 const char *number)
{
	MMSimpleConnectProperties *mm_simple_connect_properties =
		mm_simple_connect_properties_new();
	if (!mm_simple_connect_properties) {
		cm_error("Unable to allocate enough space %d", errno);
		abort();
	}
	if(operator_id)
		mm_simple_connect_properties_set_operator_id(mm_simple_connect_properties, operator_id);

	if(pin)
		mm_simple_connect_properties_set_pin(mm_simple_connect_properties, pin);
	if(apn)
		mm_simple_connect_properties_set_apn(mm_simple_connect_properties, apn);
	if(user)
		mm_simple_connect_properties_set_user(mm_simple_connect_properties, user);
	if(password)
		mm_simple_connect_properties_set_password(mm_simple_connect_properties,
							  password);
	mm_simple_connect_properties_set_allow_roaming(mm_simple_connect_properties,
						       allow_roaming);
	if (number)
		mm_simple_connect_properties_set_number(mm_simple_connect_properties, number);

	MMBearerIpFamily mm_ip_type = MM_BEARER_IP_FAMILY_NONE;
	switch(ip_type) {
	case CM_IP_TYPE_NONE:
		break;
	case CM_IP_TYPE_IPV4:
		mm_ip_type = MM_BEARER_IP_FAMILY_IPV4;
		break;
	case CM_IP_TYPE_IPV6:
		mm_ip_type = MM_BEARER_IP_FAMILY_IPV6;
		break;
	case CM_IP_TYPE_IPV4V6:
		mm_ip_type = MM_BEARER_IP_FAMILY_IPV4V6;
		break;
	case CM_IP_TYPE_ANY:
		mm_ip_type = MM_BEARER_IP_FAMILY_ANY;
		break;
	default:
		cm_warn("Invalid CMIpType setting MMBearerIpFamily-ANY");
		mm_ip_type = MM_BEARER_IP_FAMILY_ANY;
		break;
	}
	mm_simple_connect_properties_set_ip_type(mm_simple_connect_properties, mm_ip_type);
	return mm_simple_connect_properties;
}


struct cm_modem_connect_properties *
cm_modem_connect_properties_obj_new(const char *pin,
				    const char *operator_id,
				    const char *apn,
				    const char *user,
				    const char *password,
				    cm_ip_type_t ip_type,
				    unsigned int allow_roaming,
				    const char *number)
{
	struct cm_modem_connect_properties_priv *priv =
		(struct cm_modem_connect_properties_priv *)calloc(1, sizeof(*priv));
	if (!priv) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	struct cm_modem_connect_properties *self =
		(struct cm_modem_connect_properties *)calloc(1, sizeof(*self));
	if (!self) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}
	priv->mm_simple_connect_properties =
		cm_modem_connect_properties_obj_set_mm_simple_connect_properties(pin, operator_id, apn, user,
										 password,
										 ip_type,
										 allow_roaming,
										 number);
	cm_object_init(&self->cmobj);
	self->cmobj.release = &cm_modem_connect_properties_obj_release;
	cm_object_set_name(&self->cmobj, CM_MODEM_CONNECT_PROPERTIES_CLASS_NAME);

	self->priv = priv;
	self->get = &cm_modem_connect_properties_obj_get;
	self->put = &cm_modem_connect_properties_obj_put;
	self->get_path = &cm_modem_connect_properties_obj_get_path;
	self->get_apn = &cm_modem_connect_properties_obj_get_pin;
	self->get_apn = &cm_modem_connect_properties_obj_get_operator_id;
	self->get_apn = &cm_modem_connect_properties_obj_get_apn;
	self->get_user = &cm_modem_connect_properties_obj_get_user;
	self->get_password = &cm_modem_connect_properties_obj_get_password;
	self->get_allow_roaming = &cm_modem_connect_properties_obj_get_allow_roaming;
	self->get_number = &cm_modem_connect_properties_obj_get_number;
	self->get_ip_type = &cm_modem_connect_properties_obj_get_ip_type;

	return self;
}
