#ifndef _CM_MANAGER_PRIV_H_
#define _CM_MANAGER_PRIV_H_

#include "cm_atomic.h"
#include "cm_object.h"
#include "cm_module.h"
#include "cm_manager_obj.h"
#include "cm_manager_clbk_defs.h"

struct cm_manager_priv {
	/* owner is passed when creating cm_manager after
	 * dynamically loading the library i.e. dlopen
	 * cm_manager will take refernce of cm_module
	 * and release it when it gets finalized.
	 */
	struct cm_module *owner;
	struct cm_set *modems;
	cm_atomic_t num_modems;
};

void cm_manager_obj_list_modems_async(struct cm_manager *self,
				      cm_manager_list_modems_for_each for_each,
				      cm_manager_list_modems_done done,
				      void *userdata);

void cm_manager_obj_list_modems(struct cm_manager *self,
				cm_manager_list_modems_for_each for_each,
				void *userdata,
				cm_err_t *err);

void cm_manager_obj_subscribe_modem_added(struct cm_manager *self,
					  cm_manager_modem_added added,
					  void *userdata,
					  cm_err_t *err);

void cm_manager_obj_unsubscribe_modem_added(struct cm_manager *self,
					    cm_err_t *err);

void cm_manager_obj_subscribe_modem_removed(struct cm_manager *self,
					    cm_manager_modem_removed removed,
					    void *userdata,
					    cm_err_t *err);

void cm_manager_obj_unsubscribe_modem_removed(struct cm_manager *self,
					      cm_err_t *err);

#endif /* _CM_MANAGER_PRIV_H_ */
