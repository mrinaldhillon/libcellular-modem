#ifndef _CM_BEARER_CLBK_DEFS_H_
#define _CM_BEARER_CLBK_DEFS_H_

#include "cm_err.h"
#include "cm_bearer_state.h"

struct cm_bearer;
struct cm_bearer_ip_config;
struct cm_bearer_stats;

typedef void (*cm_bearer_new_done)(struct cm_bearer *bearer,
				   void *userdata,
				   cm_err_t err);

typedef void (*cm_bearer_connect_done)(struct cm_bearer *self,
			       void *userdata,
			       cm_err_t err);

typedef void (*cm_bearer_disconnect_done)(struct cm_bearer *self,
			       void *userdata,
			       cm_err_t err);

typedef void (*cm_bearer_get_stats_done)(struct cm_bearer *self,
				 struct cm_bearer_stats *stats,
				 void *userdata,
				 cm_err_t err);

typedef void (*cm_bearer_get_ip_config_done)(struct cm_bearer *self,
				 struct cm_bearer_ip_config *ip_config,
				 void *userdata,
				 cm_err_t err);

typedef void (*cm_bearer_get_interface_done)(struct cm_bearer *self,
				 const char *interface,
				 void *userdata,
				 cm_err_t err);

typedef void (*cm_bearer_state_updated)(struct cm_bearer *self,
				 cm_bearer_state_t state,
				 void *userdata,
				 cm_err_t err);

typedef void (*cm_bearer_stats_updated)(struct cm_bearer *self,
				 struct cm_bearer_stats *stats,
				 void *userdata,
				 cm_err_t err);

typedef void (*cm_bearer_ip_config_updated)(struct cm_bearer *self,
				 struct cm_bearer_ip_config *ip_config,
				 void *userdata,
				 cm_err_t err);

typedef void (*cm_bearer_interface_updated)(struct cm_bearer *self,
				 const char *interface,
				 void *userdata,
				 cm_err_t err);

#endif /* _CM_BEARER_CLBK_DEFS_H_ */
