#include <assert.h>
#include "cm_ip_method.h"
#include "cm_bearer_ip_config_obj.h"

struct cm_bearer_ip_config *
cm_bearer_ip_config_ref(struct cm_bearer_ip_config *self)
{
	assert(self && self->get);
	return self->get(self);
}

void cm_bearer_ip_config_unref(struct cm_bearer_ip_config *self)
{
	assert(self && self->put);
	self->put(self);
}

cm_ip_method_t cm_bearer_ip_config_get_method(struct cm_bearer_ip_config *self)
{
	assert(self && self->get_method);
	return self->get_method(self);
}

const char * cm_bearer_ip_config_get_address(struct cm_bearer_ip_config *self)
{
	assert(self && self->get_address);
	return self->get_address(self);
}

unsigned int cm_bearer_ip_config_get_prefix(struct cm_bearer_ip_config *self)
{
	assert(self && self->get_prefix);
	return self->get_prefix(self);
}

const char * cm_bearer_ip_config_get_dns(struct cm_bearer_ip_config *self)
{
	assert(self && self->get_dns);
	return self->get_dns(self);
}

const char * cm_bearer_ip_config_get_gateway(struct cm_bearer_ip_config *self)
{
	assert(self && self->get_gateway);
	return self->get_gateway(self);
}

unsigned int cm_bearer_ip_config_get_mtu(struct cm_bearer_ip_config *self)
{
	assert(self && self->get_mtu);
	return self->get_mtu(self);
}
