#ifndef _CM_MANAGER_
#define _CM_MANAGER_

#include "cm_err.h"
#include "cm_modem.h"

struct cm_manager;

typedef void (*cm_manager_modem_added)(struct cm_manager *self,
				       struct cm_modem *modem,
				       void *userdata);
typedef void (*cm_manager_modem_removed)(struct cm_manager *self,
					 struct cm_modem *modem,
					 void *userdata);

typedef void (*cm_manager_list_modems_for_each)(struct cm_manager *self,
				       struct cm_modem *modem,
				       void *userdata);
typedef void (*cm_manager_list_modems_done)(struct cm_manager *self,
				       void *userdata,
				       cm_err_t err);

typedef void(*cm_manager_start_done)(struct cm_manager *self,
				void *userdata,
				cm_err_t err);
typedef void(*cm_manager_stop_done)(struct cm_manager *self,
				void *userdata,
				cm_err_t err);

struct cm_manager * cm_manager_new(cm_err_t *err);

struct cm_manager * cm_manager_ref(struct cm_manager *self);

void cm_manager_unref(struct cm_manager *self);

void cm_manager_destroy(struct cm_manager *self);

/*
 * Return /CM_MANAGER
 */
const char * cm_manager_get_path(struct cm_manager *self);

void cm_manager_start(struct cm_manager *self,
		      cm_err_t *err);

void cm_manager_start_async(struct cm_manager *self,
			    cm_manager_start_done done,
			    void *userdata);

void cm_manager_stop(struct cm_manager *self,
		     cm_err_t *err);

void cm_manager_stop_async(struct cm_manager *self,
			   cm_manager_stop_done done,
			   void *userdata);

void cm_manager_list_modems(struct cm_manager *self,
			    cm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err);

void cm_manager_list_modems_async(struct cm_manager *self,
				  cm_manager_list_modems_for_each for_each,
				  cm_manager_list_modems_done done,
				  void *userdata);

void cm_manager_subscribe_modem_added(struct cm_manager *self,
				      cm_manager_modem_added added,
				      void *userdata,
				      cm_err_t *err);
void cm_manager_unsubscribe_modem_added(struct cm_manager *self,
					cm_err_t *err);

void cm_manager_subscribe_modem_removed(struct cm_manager *self,
					cm_manager_modem_removed removed,
					void *userdata,
					cm_err_t *err);
void cm_manager_unsubscribe_modem_removed(struct cm_manager *self,
					  cm_err_t *err);

#endif /* _CM_MANAGER_ */
