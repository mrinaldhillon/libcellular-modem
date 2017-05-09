#ifndef _CMM_MANAGER_H_
#define _CMM_MANAGER_H_

#include "cm_err.h"
#include "cmm_manager_clbk_defs.h"

struct cmm_manager;

struct cmm_manager * cmm_manager_new(cm_err_t *err);

void cmm_manager_new_async(cmm_manager_new_done done,
			   void *userdata);

struct cmm_manager * cmm_manager_ref(struct cmm_manager *self);

void cmm_manager_unref(struct cmm_manager *self);

void cmm_manager_destroy(struct cmm_manager *self);

/*
 * Return /CM_MANAGER
 */
char * cmm_manager_get_path(struct cmm_manager *self);

void cmm_manager_start(struct cmm_manager *self,
		      cm_err_t *err);

void cmm_manager_start_async(struct cmm_manager *self,
			    cmm_manager_start_done done,
			    void *userdata);

void cmm_manager_stop(struct cmm_manager *self,
		     cm_err_t *err);

void cmm_manager_stop_async(struct cmm_manager *self,
			   cmm_manager_stop_done done,
			   void *userdata);

void cmm_manager_list_modems(struct cmm_manager *self,
			    cmm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err);

void cmm_manager_list_modems_async(struct cmm_manager *self,
				  cmm_manager_list_modems_for_each for_each,
				  cmm_manager_list_modems_done done,
				  void *userdata);

void cmm_manager_subscribe_modem_added(struct cmm_manager *self,
				      cmm_manager_modem_added added,
				      void *userdata,
				      cm_err_t *err);

void cmm_manager_unsubscribe_modem_added(struct cmm_manager *self,
					cm_err_t *err);

void cmm_manager_subscribe_modem_removed(struct cmm_manager *self,
					cmm_manager_modem_removed removed,
					void *userdata,
					cm_err_t *err);

void cmm_manager_unsubscribe_modem_removed(struct cmm_manager *self,
					  cm_err_t *err);

#endif /* _CMM_MANAGER_H_ */
