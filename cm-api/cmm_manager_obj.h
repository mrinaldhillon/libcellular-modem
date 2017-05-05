#ifndef _CMM_MANAGER_OBJ_H_
#define _CMM_MANAGER_OBJ_H_

#include "cm_atomic.h"
#include "cm_object.h"
#include "cm_err.h"
#include "cmm_manager_clbk_defs.h"

struct cmm_manager_priv;

struct cmm_manager {
	struct cm_object cmobj;
	struct cmm_manager_priv *priv;

	/* needed before final unref */
	void (*cleanup)(struct cmm_manager *self);

	void (*start)(struct cmm_manager *self, cm_err_t *err);

	void (*start_async)(struct cmm_manager *self,
			    cmm_manager_start_done done,
			    void *userdata);

	void (*stop)(struct cmm_manager *self, cm_err_t *err);

	void (*stop_async)(struct cmm_manager *self,
			   cmm_manager_start_done done,
			   void *userdata);

	void (*list_modems)(struct cmm_manager *self,
			    cmm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err);

	void (*list_modems_async)(struct cmm_manager *self,
				  cmm_manager_list_modems_for_each for_each,
				  cmm_manager_list_modems_done done,
				  void *userdata);

	void (*subscribe_modem_added)(struct cmm_manager *self,
				      cmm_manager_modem_added added,
				      void *userdata,
				      cm_err_t *err);

	void (*unsubscribe_modem_added)(struct cmm_manager *self,
					cm_err_t *err);

	void (*subscribe_modem_removed)(struct cmm_manager *self,
					cmm_manager_modem_removed removed,
					void *userdata,
					cm_err_t *err);

	void (*unsubscribe_modem_removed)(struct cmm_manager *self,
					  cm_err_t *err);
};

static inline struct cmm_manager * to_cmm_manager(struct cm_object *cmobj)
{
	return cmobj ? cm_container_of(cmobj, struct cmm_manager, cmobj) : NULL;
}

struct cmm_manager * cmm_manager_obj_new(cm_err_t *err);
#if 0
#if !defined(_CMM_MANAGER_H_)

#define cmm_manager_new		cmm_manager_obj_new
#endif /* !defined(_CMM_MANAGER_H_) */
#endif

#endif /* _CMM_MANAGER_OBJ_H_ */
