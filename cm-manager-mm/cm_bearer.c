#include <assert.h>
#include "cm_bearer_obj.h"
#include "cm_bearer_properties_obj.h"
#include "cm_bearer_stats_obj.h"
#include "cm_bearer_ip_config_obj.h"
#include "cm_bearer_clbk_defs.h"

struct cm_bearer * cm_bearer_ref(struct cm_bearer *self)
{
	assert(self && self->get);
	return self->get(self);
}

void cm_bearer_unref(struct cm_bearer *self)
{
	assert(self && self->put);
	self->put(self);
}

char * cm_bearer_get_path(struct cm_bearer *self)
{
	assert(self && self->get_path);
	return self->get_path(self);
}

/*
 * Not sure if this is required, since parent path can be parsed
 * from the bearer path i.e. ../Modem#/Bearer#
 */
char * cm_bearer_get_modem_path(struct cm_bearer *self);

unsigned int cm_bearer_is_connected(struct cm_bearer *self)
{
	assert(self && self->is_connected);
	return self->is_connected(self);
}

void cm_bearer_connect(struct cm_bearer *self, cm_err_t *err)
{
	assert(self && self->connect);
	self->connect(self, err);
}

void cm_bearer_connect_async(struct cm_bearer *self,
			     cm_bearer_connect_done done,
			     void *userdata)
{
	assert(self && self->connect_async);
	self->connect_async(self, done, userdata);
}

void cm_bearer_disconnect(struct cm_bearer *self, cm_err_t *err)
{
	assert(self && self->disconnect);
	self->disconnect(self, err);
}

void cm_bearer_disconnect_async(struct cm_bearer *self,
			     cm_bearer_disconnect_done done,
			     void *userdata)
{
	assert(self && self->connect_async);
	self->disconnect_async(self, done, userdata);
}

struct cm_bearer_properties * cm_bearer_get_properties(struct cm_bearer *self)
{
	assert(self && self->get_properties);
	return self->get_properties(self);
}

/*
 * @tbd:
 * For now stats are not local property and will be fetched from base manager
 * over ipc hence the err and asyc callback. Later we will implement an update
 * event handler per live object, that will allow the live object to locally
 * update its properties based on notifications. So property returned will be
 * most updated and will not require async call as well as error handling.
 */
struct cm_bearer_stats * cm_bearer_get_stats(struct cm_bearer *self,
						 cm_err_t *err)
{
	assert(self && self->get_stats);
	return self->get_stats(self, err);
}

struct cm_bearer_stats *
cm_bearer_get_stats_async(struct cm_bearer *self,
			  cm_bearer_get_stats_done done,
			  void *userdata)
{
	assert(self && self->get_stats_async);
	return self->get_stats_async(self, done, userdata);
}

struct cm_bearer_ip_config * cm_bearer_get_ip_config(struct cm_bearer *self,
						     cm_err_t *err)
{
	assert(self && self->get_ip_config);
	return self->get_ip_config(self, err);
}

struct cm_bearer_ip_config *
cm_bearer_get_ip_config_async(struct cm_bearer *self,
			  cm_bearer_get_ip_config_done done,
			  void *userdata)
{
	assert(self && self->get_ip_config_async);
	return self->get_ip_config_async(self, done, userdata);
}

const char * cm_bearer_get_interface(struct cm_bearer *self,
				     cm_err_t *err)
{
	assert(self && self->get_interface);
	return self->get_interface(self, err);
}

const char * cm_bearer_get_interface_async(struct cm_bearer *self,
					   cm_bearer_get_interface_done done,
					   void *userdata)
{
	assert(self && self->get_interface_async);
	return self->get_interface_async(self, done, userdata);
}

void cm_bearer_subscribe_state_update(struct cm_bearer *self,
				      cm_bearer_stats_updated updated,
				      void *userdata,
				      cm_err_t *err)
{
	assert(self && self->subscribe_state_update);
	self->subscribe_state_update(self, updated, userdata, err);
}

void cm_bearer_unsubscribe_state_update(struct cm_bearer *self,
				      cm_err_t *err)
{
	assert(self && self->unsubscribe_state_update);
	self->unsubscribe_state_update(self, err);
}

void cm_bearer_subscribe_stats_update(struct cm_bearer *self,
				      cm_bearer_stats_updated updated,
				      void *userdata,
				      cm_err_t *err)
{
	assert(self && self->subscribe_stats_update);
	self->subscribe_stats_update(self, updated, userdata, err);
}

void cm_bearer_unsubscribe_stats_update(struct cm_bearer *self,
				      cm_err_t *err)
{
	assert(self && self->unsubscribe_stats_update);
	self->unsubscribe_stats_update(self, err);
}

void cm_bearer_subscribe_ip_config_update(struct cm_bearer *self,
					  cm_bearer_ip_config_updated updated,
					  void *userdata,
					  cm_err_t *err)
{
	assert(self && self->subscribe_ip_config_update);
	self->subscribe_ip_config_update(self, updated, userdata, err);
}

void cm_bearer_unsubscribe_ip_config_update(struct cm_bearer *self,
					  cm_err_t *err)
{
	assert(self && self->unsubscribe_ip_config_update);
	self->unsubscribe_ip_config_update(self, err);
}

void cm_bearer_subscribe_interface_update(struct cm_bearer *self,
					  cm_bearer_interface_updated updated,
					  void *userdata,
					  cm_err_t *err)
{
	assert(self && self->subscribe_interface_update);
	self->subscribe_interface_update(self, updated, userdata, err);
}

void cm_bearer_unsubscribe_interface_update(struct cm_bearer *self,
					  cm_err_t *err)
{
	assert(self && self->unsubscribe_interface_update);
	self->unsubscribe_interface_update(self, err);
}
