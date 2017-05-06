#ifndef _CM_MANAGER_CLBK_DEFS_H_
#define _CM_MANAGER_CLBK_DEFS_H_

#include "cm_err.h"
#include "cm_modem.h"

struct cm_manager;

typedef void (*cm_manager_new_done)(struct cm_manager *manager,
				    void *userdata,
				    cm_err_t err);

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


#endif /* _CM_MANAGER_CLBK_DEFS_H_ */
