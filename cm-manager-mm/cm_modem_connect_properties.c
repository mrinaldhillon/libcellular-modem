#include <assert.h>
#include "cm_modem_connect_properties.h"
#include "cm_modem_connect_properties_obj.h"

struct cm_modem_connect_properties *
cm_modem_connect_properties_new(const char *pin,
				const char *operator_id,
				const char *apn,
				const char *user,
				const char *password,
				cm_ip_type_t ip_type,
				unsigned int allow_roaming,
				const char *number)
{
	return cm_modem_connect_properties_obj_new(pin, operator_id, apn,
						   user, password,
						   ip_type, allow_roaming,
						   number);
}

// @todo: provide set methods
struct cm_modem_connect_properties *
cm_modem_connect_properties_ref(struct cm_modem_connect_properties *self)
{
	assert(self && self->get);
	return self->get(self);
}

void cm_modem_connect_properties_unref(struct cm_modem_connect_properties *self)
{
	assert(self && self->put);
	self->put(self);
}

const char *
cm_modem_connect_properties_get_pin(struct cm_modem_connect_properties *self)
{
	assert(self && self->get_pin);
	return self->get_pin(self);
}

const char *
cm_modem_connect_properties_get_operator_id(struct
					    cm_modem_connect_properties *self)
{
	assert(self && self->get_operator_id);
	return self->get_operator_id(self);
}

const char *
cm_modem_connect_properties_get_apn(struct cm_modem_connect_properties *self)
{
	assert(self && self->get_apn);
	return self->get_apn(self);
}

const char *
cm_modem_connect_properties_get_user(struct cm_modem_connect_properties *self)
{
	assert(self && self->get_user);
	return self->get_user(self);
}

const char *
cm_modem_connect_properties_get_password(struct
					 cm_modem_connect_properties *self)
{
	assert(self && self->get_password);
	return self->get_password(self);
}

cm_ip_type_t
cm_modem_connect_properties_get_ip_type(struct
					cm_modem_connect_properties *self)
{
	assert(self && self->get_ip_type);
	return self->get_ip_type(self);
}

unsigned int
cm_modem_connect_properties_get_allow_roaming(struct
					      cm_modem_connect_properties *self)
{
	assert(self && self->get_allow_roaming);
	return self->get_allow_roaming(self);
}

const char *
cm_modem_connect_properties_get_number(struct cm_modem_connect_properties *self)
{
	assert(self && self->get_number);
	return self->get_number(self);
}
