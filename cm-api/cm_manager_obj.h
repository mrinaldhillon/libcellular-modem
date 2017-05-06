#ifndef _CM_MANAGER_OBJ_H_
#define _CM_MANAGER_OBJ_H_

#include "cm_object.h"
#include "cm_module.h"
#include "cm_err.h"
#include "cm_manager_clbk_defs.h"

struct cm_manager_priv;

struct cm_manager {
	struct cm_object cmobj;
	struct cm_manager_priv *priv;

	const char *(*get_class_name)(void);

	struct cm_manager *(*get)(struct cm_manager *self);

	void (*put)(struct cm_manager *self);

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

/*
 * owner: is passed only if library is loaded dynamically ie. dlopen
 * it is required to not allow library to unload while its objects
 * are still in memory, provide NULL otherwise */
struct cm_manager * cm_manager_obj_new(struct cm_module *owner,
				       cm_err_t *err);

void cm_manager_obj_new_async(struct cm_module *owner,
			      cm_manager_new_done done,
			      void *userdata);


#if 0
#if !defined(_CM_MANAGER_H_)

#define cm_manager_new cm_manager_obj_new
struct cm_manager * cm_manager_new(cm_err_t *err);
#endif /* !defined(_CM_MANAGER_H_) */
#endif

#endif /* _CM_MANAGER_OBJ_H_ */
