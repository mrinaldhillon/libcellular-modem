#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <libmm-glib.h>
#include "cm_log.h"
#include "cm_err.h"
#include "cm_object.h"
#include "cm_ip_method.h"
#include "cm_bearer_ip_config_obj.h"

static const char *
cm_bearer_ip_config_obj_get_address(struct cm_bearer_ip_config *self)
{
	const char * address = mm_bearer_ip_config_get_address(self->mm_ip_config);
	return address;
}

static const char *
cm_bearer_ip_config_obj_get_dns(struct cm_bearer_ip_config *self)
{
	return (const char *)
		mm_bearer_ip_config_get_dns(self->mm_ip_config);
}

static const char *
cm_bearer_ip_config_obj_get_gateway(struct cm_bearer_ip_config *self)
{
	return (const char *)
		mm_bearer_ip_config_get_gateway(self->mm_ip_config);
}

static unsigned int
cm_bearer_ip_config_obj_get_prefix(struct cm_bearer_ip_config *self)
{
	return (unsigned int)
		mm_bearer_ip_config_get_prefix(self->mm_ip_config);
}

static unsigned int
cm_bearer_ip_config_obj_get_mtu(struct cm_bearer_ip_config *self)
{
	return (unsigned int)
		mm_bearer_ip_config_get_mtu(self->mm_ip_config);
}

static cm_ip_method_t
cm_bearer_ip_config_obj_get_method(struct cm_bearer_ip_config *self)
{
	MMBearerIpMethod ip_method =
		mm_bearer_ip_config_get_method(self->mm_ip_config);

	switch(ip_method) {
	case MM_BEARER_IP_METHOD_UNKNOWN:
		return CM_IP_METHOD_UNKNOWN;
	case MM_BEARER_IP_METHOD_PPP:
		return CM_IP_METHOD_PPP;
	case MM_BEARER_IP_METHOD_STATIC:
		return CM_IP_METHOD_STATIC;
	case MM_BEARER_IP_METHOD_DHCP:
		return CM_IP_METHOD_DHCP;
	default:
		cm_warn("Invalid MMBearerIpMethod check ModemManager doc");
		return CM_IP_METHOD_UNKNOWN;
	}
}

static struct cm_bearer_ip_config *
cm_bearer_ip_config_obj_get(struct cm_bearer_ip_config *self)
{
	assert(self);

	cm_object_get(&self->cmobj);
	return self;
}

static void cm_bearer_ip_config_obj_put(struct cm_bearer_ip_config *self)
{
	assert(self);
	cm_object_put(&self->cmobj);
}

static void cm_bearer_ip_config_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_bearer_ip_config *self = to_cm_bearer_ip_config(cmobj);

	cm_debug("Destroying %s", cm_object_get_name(cmobj));
	if (self->mm_ip_config)
		g_object_unref(self->mm_ip_config);

	free(self);
}

struct cm_bearer_ip_config *
cm_bearer_ip_config_obj_new(MMBearerIpConfig *mm_ip_config)
{
	struct cm_bearer_ip_config *self =
		(struct cm_bearer_ip_config *)calloc(1, sizeof(*self));
	if (!self) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	cm_object_init(&self->cmobj);
	self->cmobj.release = &cm_bearer_ip_config_obj_release;
	cm_object_set_name(&self->cmobj, CM_BEARER_IP_CONFIG_CLASS_NAME);
	self->mm_ip_config = g_object_ref(mm_ip_config);

	self->get = &cm_bearer_ip_config_obj_get;
	self->put = &cm_bearer_ip_config_obj_put;
	self->get_address = &cm_bearer_ip_config_obj_get_address;
	self->get_prefix = &cm_bearer_ip_config_obj_get_prefix;
	self->get_dns = &cm_bearer_ip_config_obj_get_dns;
	self->get_mtu = &cm_bearer_ip_config_obj_get_mtu;
	self->get_method = &cm_bearer_ip_config_obj_get_method;
	self->get_gateway = &cm_bearer_ip_config_obj_get_gateway;

	return self;
}
