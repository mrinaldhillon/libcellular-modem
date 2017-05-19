#ifndef _CM_MODEM_CLBK_DEFS_H_
#define _CM_MODEM_CLBK_DEFS_H_

#include "cm_err.h"
#include "cm_modem_state.h"

struct cm_modem;
struct cm_bearer;
struct cm_signal;

typedef void (*cm_modem_new_done)(struct cm_modem *self,
				     void *userdata,
				     cm_err_t err);

typedef void (*cm_modem_enable_done)(struct cm_modem *self,
				     void *userdata,
				     cm_err_t err);

typedef void (*cm_modem_disable_done)(struct cm_modem *self,
				      void *userdata,
				      cm_err_t err);

typedef void (*cm_modem_connect_done)(struct cm_modem *self,
				      void *userdata,
				      cm_err_t err);

typedef void (*cm_modem_disconnect_done)(struct cm_modem *self,
					 void *userdata,
					 cm_err_t err);

typedef void (*cm_modem_get_state_done)(struct cm_modem *self,
					cm_modem_state_t state,
					void *userdata,
					cm_err_t err);

typedef void (*cm_modem_get_signal_done)(struct cm_modem *self,
					 struct cm_signal *signal,
					 void *userdata,
					 cm_err_t err);

typedef void (*cm_modem_create_bearer_done)(struct cm_modem *self,
					    struct cm_bearer *bearer,
					    void *userdata,
					    cm_err_t err);

typedef void (*cm_modem_delete_bearer_done)(struct cm_modem *self,
					    void *userdata,
					    cm_err_t err);

typedef void (*cm_modem_list_bearers_for_each)(struct cm_modem *self,
					       struct cm_bearer *bearer,
					       void *userdata);

typedef void (*cm_modem_list_bearers_done)(struct cm_modem *self,
					   void *userdata,
					   cm_err_t err);

typedef void (*cm_modem_state_updated)(struct cm_modem *self,
				       cm_modem_state_t state,
				       void *userdata,
				       cm_err_t err);

typedef void (*cm_modem_signal_updated)(struct cm_modem *self,
					struct cm_signal *signal,
					void *userdata,
					cm_err_t err);

#endif /* _CM_MODEM_CLBK_DEFS_H_ */

