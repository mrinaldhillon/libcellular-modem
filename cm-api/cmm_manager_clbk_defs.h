#ifndef _CMM_MANAGER_CLBK_DEFS_H_
#define _CMM_MANAGER_CLBK_DEFS_H_

#include "cm_err.h"
#include "cm_modem.h"

struct cmm_manager;

typedef void (*cmm_manager_new_done)(struct cmm_manager *manager,
				     void *userdata,
				     cm_err_t err);

typedef void (*cmm_manager_modem_added)(struct cmm_manager *self,
					struct cm_modem *modem,
					void *userdata);

typedef void (*cmm_manager_modem_removed)(struct cmm_manager *self,
					  struct cm_modem *modem,
					  void *userdata);

typedef void (*cmm_manager_list_modems_for_each)(struct cmm_manager *self,
						 struct cm_modem *modem,
						 void *userdata);
typedef void (*cmm_manager_list_modems_done)(struct cmm_manager *self,
					     void *userdata,
					     cm_err_t err);

typedef void(*cmm_manager_start_done)(struct cmm_manager *self,
				      void *userdata,
				      cm_err_t err);

typedef void(*cmm_manager_stop_done)(struct cmm_manager *self,
				     void *userdata,
				     cm_err_t err);


#endif /* _CMM_MANAGER_CLBK_DEFS_H_ */
