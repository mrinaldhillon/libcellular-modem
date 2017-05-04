#ifndef _CM_MANAGER_OBJ_H_
#define _CM_MANAGER_OBJ_H_

#include "cm_atomic.h"
#include "cm_object.h"
#include "cm_err.h"
#include "cm_manager_clbk_defs.h"

struct cm_manager_priv;


struct cm_manager {
	struct cm_object cmobj;
	struct cm_manager_priv *priv;

	void (*cleanup)(struct cm_manager *self);

	void (*start)(struct cm_manager *self, cm_err_t *err);

	void (*start_async)(struct cm_manager *self,
			    cm_manager_start_done done,
			    void *userdata);

	void (*stop)(struct cm_manager *self, cm_err_t *err);

	void (*stop_async)(struct cm_manager *self,
			   cm_manager_start_done done,
			   void *userdata);

	void (*list_modems)(struct cm_manager *self,
			    cm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err);

	void (*list_modems_async)(struct cm_manager *self,
				  cm_manager_list_modems_for_each for_each,
				  cm_manager_list_modems_done done,
				  void *userdata);

	void (*subscribe_modem_added)(struct cm_manager *self,
				      cm_manager_modem_added added,
				      void *userdata,
				      cm_err_t *err);

	void (*unsubscribe_modem_added)(struct cm_manager *self,
					cm_err_t *err);

	void (*subscribe_modem_removed)(struct cm_manager *self,
					cm_manager_modem_removed removed,
					void *userdata,
					cm_err_t *err);

	void (*unsubscribe_modem_removed)(struct cm_manager *self,
					  cm_err_t *err);
};

static inline struct cm_manager * to_cm_manager(struct cm_object *cmobj)
{
	return cmobj ? cm_container_of(cmobj, struct cm_manager, cmobj) : NULL;
}

struct cm_manager * cm_manager_obj_new(cm_err_t *err);

#if !defined(_CM_MANAGER_H_)

struct cm_manager * cm_manager_new(cm_err_t *err);

#endif /* !defined(_CM_MANAGER_H_) */


#endif /* _CMM_MANAGER_OBJ_H_ */
