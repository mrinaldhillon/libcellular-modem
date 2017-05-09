#include <assert.h>
#include "cm_err.h"
#include "cm_object.h"
#include "cmm_manager.h"
#include "cmm_manager_obj.h"

struct cmm_manager * cmm_manager_new(cm_err_t *err)
{
	return cmm_manager_obj_new(err);
}

void cmm_manager_new_async(cmm_manager_new_done done,
			   void *userdata)
{
	cmm_manager_obj_new_async(done, userdata);
}

struct cmm_manager * cmm_manager_ref(struct cmm_manager *self)
{
	assert(self);
	return self->get(self);
}

void cmm_manager_unref(struct cmm_manager *self)
{
	assert(self);
	self->put(self);
}

char * cmm_manager_get_path(struct cmm_manager *self)
{
	assert(self);
	return cm_object_get_path(&self->cmobj);
}

void cmm_manager_start(struct cmm_manager *self, cm_err_t *err)
{
	assert(self);
	return self->start(self, err);
}

void cmm_manager_start_async(struct cmm_manager *self,
			    cmm_manager_start_done done,
			    void *userdata)
{
	assert(self);
	return self->start_async(self, done, userdata);
}

void cmm_manager_stop(struct cmm_manager *self, cm_err_t *err)
{
	assert(self);
	return self->stop(self, err);
}

void cmm_manager_stop_async(struct cmm_manager *self,
			   cmm_manager_stop_done done,
			   void *userdata)
{
	assert(self);
	return self->stop_async(self, done, userdata);
}

void cmm_manager_list_modems(struct cmm_manager *self,
			    cmm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err)
{
	assert(self);
	return self->list_modems(self, for_each, userdata, err);
}

void cmm_manager_list_modems_async(struct cmm_manager *self,
				  cmm_manager_list_modems_for_each for_each,
				  cmm_manager_list_modems_done done,
				  void *userdata)
{
	assert(self);
	return self->list_modems_async(self, for_each, done, userdata);
}

void cmm_manager_subscribe_modem_added(struct cmm_manager *self,
				      cmm_manager_modem_added added,
				      void *userdata,
				      cm_err_t *err)
{
	assert(self);
	return self->subscribe_modem_added(self, added, userdata, err);
}

void cmm_manager_unsubscribe_modem_added(struct cmm_manager *self,
					cm_err_t *err)
{
	assert(self);
	return self->unsubscribe_modem_added(self, err);
}

void cmm_manager_subscribe_modem_removed(struct cmm_manager *self,
					cmm_manager_modem_removed removed,
					void *userdata,
					cm_err_t *err)
{
	assert(self);
	return self->subscribe_modem_removed(self, removed, userdata, err);
}

void cmm_manager_unsubscribe_modem_removed(struct cmm_manager *self,
					  cm_err_t *err)
{
	assert(self);
	return self->unsubscribe_modem_removed(self, err);

}
