#include <assert.h>
#include "cm_err.h"
#include "cm_modem_state.h"
#include "cm_signal_obj.h"
#include "cm_bearer_obj.h"
#include "cm_bearer_properties_obj.h"
//#include "cm_sim.h"
//#include "cm_messaging.h"
#include "cm_modem_clbk_defs.h"
#include "cm_modem.h"
#include "cm_modem_obj.h"

struct cm_modem * cm_modem_ref(struct cm_modem *self)
{
	assert(self && self->get);
	return self->get(self);
}

void cm_modem_unref(struct cm_modem *self)
{
	assert(self && self->put);
	self->put(self);
}

char * cm_modem_get_path(struct cm_modem *self)
{
	assert(self && self->get_path);
	return self->get_path(self);
}

const char * cm_modem_get_manufacturer(struct cm_modem *self)
{
	assert(self && self->get_manufacturer);
	return self->get_manufacturer(self);
}

const char * cm_modem_get_model(struct cm_modem *self)
{
	assert(self && self->get_model);
	return self->get_model(self);
}

const char * cm_modem_get_serial(struct cm_modem *self)
{
	assert(self && self->get_serial);
	return self->get_serial(self);
}

void cm_modem_enable(struct cm_modem *self, cm_err_t *err)
{
	assert(self && self->enable);
	self->enable(self, err);
}

void cm_modem_enable_async(struct cm_modem *self,
			   cm_modem_enable_done done,
			   void *userdata)
{
	assert(self && self->enable_async);
	self->enable_async(self, done, userdata);
}

void cm_modem_disable(struct cm_modem *self, cm_err_t *err)
{
	assert(self && self->disable);
	self->disable(self, err);
}

void cm_modem_disable_async(struct cm_modem *self,
			   cm_modem_disable_done done,
			   void *userdata)
{
	assert(self && self->disable_async);
	self->disable_async(self, done, userdata);
}

cm_modem_state_t cm_modem_get_state(struct cm_modem *self,
				    cm_err_t *err)
{
	assert(self && self->get_state);
	return self->get_state(self, err);
}

void cm_modem_get_state_async(struct cm_modem *self,
			      cm_modem_get_state_done done,
			      void *userdata)
{
	assert(self && self->get_state_async);
	self->get_state_async(self, done, userdata);
}

struct cm_bearer * cm_modem_connect(struct cm_modem *self,
				    struct cm_bearer_properties *properties,
				    cm_err_t *err)
{
	assert(self && self->connect);
	return self->connect(self, properties, err);
}

void cm_modem_connect_async(struct cm_modem *self,
				    struct cm_bearer_properties *properties,
				    cm_modem_connect_done done,
				    void *userdata)
{
	assert(self && self->connect_async);
	return self->connect_async(self, properties, done, userdata);
}

void cm_modem_disconnect(struct cm_modem *self,
			 const char *bearer_path,
			 cm_err_t *err)
{
	assert(self && self->disconnect);
	return self->disconnect(self, bearer_path, err);
}

void cm_modem_disconnect_async(struct cm_modem *self,
			 const char *bearer_path,
			 cm_modem_disconnect_done done,
			 void *userdata)
{
	assert(self && self->disconnect_async);
	return self->disconnect_async(self, bearer_path, done, userdata);
}

struct cm_signal * cm_modem_get_signal(struct cm_modem *self,
				       cm_err_t *err)
{
	assert(self && self->get_signal);
	return self->get_signal(self, err);
}

struct cm_bearer *
cm_modem_create_bearer(struct cm_modem *self,
		       struct cm_bearer_properties *properties,
		       cm_err_t *err)
{
	assert(self && self->create_bearer);
	return self->create_bearer(self, properties, err);
}

void cm_modem_create_bearer_async(struct cm_modem *self,
		       struct cm_bearer_properties *properties,
		       cm_modem_create_bearer_done done,
		       void *userdata)
{
	assert(self && self->create_bearer_async);
	return self->create_bearer_async(self, properties, done, userdata);
}

void cm_modem_delete_bearer(struct cm_modem *self,
			    const char *bearer_path,
			    cm_err_t *err)
{
	assert(self && self->delete_bearer);
	self->delete_bearer(self, bearer_path, err);
}

void cm_modem_delete_bearer_async(struct cm_modem *self,
			    const char *bearer_path,
			    cm_modem_delete_bearer_done done,
			    void *userdata)
{
	assert(self && self->delete_bearer_async);
	self->delete_bearer_async(self, bearer_path, done, userdata);
}

void cm_modem_list_bearers(struct cm_modem *self,
			   cm_modem_list_bearers_for_each for_each,
			   cm_modem_list_bearers_done done,
			   void *userdata,
			   cm_err_t *err)
{
	assert(self && self->list_bearers);
	self->list_bearers(self, for_each, done, userdata, err);
}

void cm_modem_list_bearers_async(struct cm_modem *self,
				 cm_modem_list_bearers_for_each for_each,
				 cm_modem_list_bearers_done done,
				 void *userdata)
{
	assert(self && self->list_bearers_async);
	self->list_bearers_async(self, for_each, done, userdata);
}

void cm_modem_subscribe_state_update(struct cm_modem *self,
				     cm_modem_state_updated updated,
				     void *userdata,
				     cm_err_t *err)
{
	assert(self && self->subscribe_state_update);
	self->subscribe_state_update(self, updated, userdata, err);
}

void cm_modem_unsubscribe_state_update(struct cm_modem *self,
				       cm_err_t *err)
{
	assert(self && self->unsubscribe_state_update);
	self->unsubscribe_state_update(self, err);
}

void cm_modem_subscribe_signal_update(struct cm_modem *self,
				      cm_modem_signal_updated updated,
				      void *userdata,
				      cm_err_t *err)
{
	assert(self && self->subscribe_signal_update);
	self->subscribe_signal_update(self, updated, userdata, err);
}

void cm_modem_unsubscribe_signal_update(struct cm_modem *self,
				       cm_err_t *err)
{
	assert(self && self->unsubscribe_signal_update);
	self->unsubscribe_signal_update(self, err);
}
