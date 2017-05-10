#include <assert.h>
#include "cm_err.h"
#include "cm_object.h"
#include "cm_manager.h"
#include "cm_manager_obj.h"

struct cm_manager * cm_manager_new(cm_err_t *err)
{
	return cm_manager_obj_new(NULL, err);
}

void cm_manager_new_async(cm_manager_new_done done,
					void *userdata)
{
	cm_manager_obj_new_async(NULL, done, userdata);
}

struct cm_manager * cm_manager_ref(struct cm_manager *self)
{
	assert(self && self->get);
	return self->get(self);
}

void cm_manager_unref(struct cm_manager *self)
{
	assert(self && self->put);
	self->put(self);
}

char * cm_manager_get_path(struct cm_manager *self)
{
	assert(self && self->get_path);
	return self->get_path(self);
}

void cm_manager_start(struct cm_manager *self, cm_err_t *err)
{
	assert(self && self->start);
	return self->start(self, err);
}

void cm_manager_start_async(struct cm_manager *self,
			    cm_manager_start_done done,
			    void *userdata)
{
	assert(self && self->start_async);
	return self->start_async(self, done, userdata);
}

void cm_manager_stop(struct cm_manager *self, cm_err_t *err)
{
	assert(self && self->stop);
	return self->stop(self, err);
}

void cm_manager_stop_async(struct cm_manager *self,
			   cm_manager_stop_done done,
			   void *userdata)
{
	assert(self && self->stop_async);
	return self->stop_async(self, done, userdata);
}

void cm_manager_list_modems(struct cm_manager *self,
			    cm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err)
{
	assert(self && self->list_modems);
	return self->list_modems(self, for_each, userdata, err);
}

void cm_manager_list_modems_async(struct cm_manager *self,
				  cm_manager_list_modems_for_each for_each,
				  cm_manager_list_modems_done done,
				  void *userdata)
{
	assert(self && self->list_modems_async);
	return self->list_modems_async(self, for_each, done, userdata);
}

void cm_manager_subscribe_modem_added(struct cm_manager *self,
				      cm_manager_modem_added added,
				      void *userdata,
				      cm_err_t *err)
{
	assert(self && self->subscribe_modem_added);
	return self->subscribe_modem_added(self, added, userdata, err);
}

void cm_manager_unsubscribe_modem_added(struct cm_manager *self,
					cm_err_t *err)
{
	assert(self && self->unsubscribe_modem_added);
	return self->unsubscribe_modem_added(self, err);
}

void cm_manager_subscribe_modem_removed(struct cm_manager *self,
					cm_manager_modem_removed removed,
					void *userdata,
					cm_err_t *err)
{
	assert(self && self->subscribe_modem_removed);
	return self->subscribe_modem_removed(self, removed, userdata, err);
}

void cm_manager_unsubscribe_modem_removed(struct cm_manager *self,
					  cm_err_t *err)
{
	assert(self && self->unsubscribe_modem_removed);
	return self->unsubscribe_modem_removed(self, err);

}
