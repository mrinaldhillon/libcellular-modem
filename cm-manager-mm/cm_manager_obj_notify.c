#include <assert.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_manager_priv.h"
#include "cm_manager_obj.h"
#include "cm_modem.h"

struct cm_manager_subscribe_modem_added_ctx {
	struct cm_manager *self;
	cm_manager_modem_added added;
	void *userdata;
};

struct cm_manager_subscribe_modem_removed_ctx {
	struct cm_manager *self;
	cm_manager_modem_removed removed;
	void *userdata;
};

void cm_manager_obj_subscribe_modem_added(struct cm_manager *self,
					  cm_manager_modem_added added,
					  void *userdata,
					  cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager

}

void cm_manager_obj_unsubscribe_modem_added(struct cm_manager *self,
					    cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

void cm_manager_obj_subscribe_modem_removed(struct cm_manager *self,
					    cm_manager_modem_removed removed,
					    void *userdata,
					    cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

void cm_manager_obj_unsubscribe_modem_removed(struct cm_manager *self,
					      cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}
