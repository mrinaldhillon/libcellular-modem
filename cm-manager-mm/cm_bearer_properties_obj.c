#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <libmm-glib.h>
#include "cm_log.h"
#include "cm_err.h"
#include "cm_object.h"
#include "cm_ip_type.h"
#include "cm_bearer_properties_obj.h"
#include "cm_bearer_properties_priv.h"

static const char *
cm_bearer_properties_obj_get_apn(struct cm_bearer_properties *self)
{
	return (const char *)
		mm_bearer_properties_get_apn(self->priv->
						mm_bearer_properties);
}

static const char *
cm_bearer_properties_obj_get_user(struct cm_bearer_properties *self)
{
	return (const char *)
		mm_bearer_properties_get_user(self->priv->
						mm_bearer_properties);
}

static const char *
cm_bearer_properties_obj_get_password(struct cm_bearer_properties *self)
{
	return (const char *)
		mm_bearer_properties_get_password(self->priv->
						mm_bearer_properties);
}

static unsigned int
cm_bearer_properties_obj_get_allow_roaming(struct cm_bearer_properties *self)
{
	return (int)
		mm_bearer_properties_get_allow_roaming(self->priv->
						       mm_bearer_properties);
}

static const char *
cm_bearer_properties_obj_get_number(struct cm_bearer_properties *self)
{
	return (const char *)
		mm_bearer_properties_get_number(self->priv->
						mm_bearer_properties);
}

static cm_ip_type_t
cm_bearer_properties_obj_get_ip_type(struct cm_bearer_properties *self)
{
	MMBearerIpFamily ip_type =
		mm_bearer_properties_get_ip_type(self->priv->
						 mm_bearer_properties);
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

char * cm_bearer_properties_obj_get_path(struct cm_bearer_properties *self)
{
	assert(self);
	return cm_object_get_path(&self->cmobj);
}

static struct cm_bearer_properties *
cm_bearer_properties_obj_get(struct cm_bearer_properties *self)
{
	assert(self && self->priv);
	cm_object_get(&self->cmobj);

	return self;
}

static void cm_bearer_properties_obj_put(struct cm_bearer_properties *self)
{
	assert(self && self->priv);
	cm_object_put(&self->cmobj);
}

static void cm_bearer_properties_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_bearer_properties *self = to_cm_bearer_properties(cmobj);

	cm_debug("Destroying %s", cm_object_get_name(cmobj));
	if (self->priv->mm_bearer_properties)
		g_object_unref(self->priv->mm_bearer_properties);

	free(self->priv);
	free(self);
}

static MMBearerProperties *
cm_bearer_properties_obj_set_mm_bearer_properties(const char *apn,
						  const char *user,
						  const char *password,
						  cm_ip_type_t ip_type,
						  unsigned int allow_roaming,
						  const char *number)
{
	MMBearerProperties *mm_bearer_properties = mm_bearer_properties_new();
	if (!mm_bearer_properties) {
		cm_error("Unable to allocate enough space %d", errno);
		abort();
	}

	if(apn)
		mm_bearer_properties_set_apn(mm_bearer_properties, apn);
	if(user)
		mm_bearer_properties_set_user(mm_bearer_properties, user);
	if(password)
		mm_bearer_properties_set_password(mm_bearer_properties,
						  password);
	mm_bearer_properties_set_allow_roaming(mm_bearer_properties,
					       allow_roaming);
	if (number)
		mm_bearer_properties_set_number(mm_bearer_properties, number);

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
	mm_bearer_properties_set_ip_type(mm_bearer_properties, mm_ip_type);
	return mm_bearer_properties;
}


struct cm_bearer_properties *
cm_bearer_properties_obj_new(const char *apn,
			 const char *user,
			 const char *password,
			 cm_ip_type_t ip_type,
			 unsigned int allow_roaming,
			 const char *number)
{
	struct cm_bearer_properties_priv *priv =
		(struct cm_bearer_properties_priv *)calloc(1, sizeof(*priv));
	if (!priv) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	struct cm_bearer_properties *self =
		(struct cm_bearer_properties *)calloc(1, sizeof(*self));
	if (!self) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}
	priv->mm_bearer_properties =
		cm_bearer_properties_obj_set_mm_bearer_properties(apn, user,
								  password,
								  ip_type,
								  allow_roaming,
								  number);
	cm_object_init(&self->cmobj);
	self->cmobj.release = &cm_bearer_properties_obj_release;
	cm_object_set_name(&self->cmobj, CM_BEARER_PROPERTIES_CLASS_NAME);

	self->priv = priv;
	self->get = &cm_bearer_properties_obj_get;
	self->put = &cm_bearer_properties_obj_put;
	self->get_path = &cm_bearer_properties_obj_get_path;
	self->get_apn = &cm_bearer_properties_obj_get_apn;
	self->get_user = &cm_bearer_properties_obj_get_user;
	self->get_password = &cm_bearer_properties_obj_get_password;
	self->get_allow_roaming = &cm_bearer_properties_obj_get_allow_roaming;
	self->get_number = &cm_bearer_properties_obj_get_number;
	self->get_ip_type = &cm_bearer_properties_obj_get_ip_type;

	return self;
}
