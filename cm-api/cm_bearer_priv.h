#ifndef _CM_BEARER_PRIV_H_
#define _CM_BEARER_PRIV_H_

#include "cm_bearer_state.h"
#include "cm_bearer_properties.h"
#include "cm_bearer_stats.h"
#include "cm_bearer_ip_config.h"
#include "cm_bearer_clbk_defs.h"

struct cm_bearer;

struct cm_bearer_priv {
	// private data
};

char * cm_bearer_obj_get_modem_path(struct cm_bearer *self);

unsigned int cm_bearer_obj_is_connected(struct cm_bearer *self);

void cm_bearer_obj_connect(struct cm_bearer *self, cm_err_t *err);

void cm_bearer_obj_connect_async(struct cm_bearer *self,
			     cm_bearer_connect_done done,
			     void *userdata);

void cm_bearer_obj_disconnect(struct cm_bearer *self, cm_err_t *err);

void cm_bearer_obj_disconnect_async(struct cm_bearer *self,
			     cm_bearer_disconnect_done done,
			     void *userdata);

struct cm_bearer_properties *
cm_bearer_obj_get_properties(struct cm_bearer *self);

/*
 * @tbd:
 * For now stats are not local property and will be fetched from base manager
 * over ipc hence the err and asyc callback. Later we will implement an update
 * event handler per live object, that will allow the live object to locally
 * update its properties based on notifications. So property returned will be
 * most updated and will not require async call as well as error handling.
 */
struct cm_bearer_stats * cm_bearer_obj_get_stats(struct cm_bearer_stats *self,
						 cm_err_t *err);
struct cm_bearer_stats *
cm_bearer_obj_get_stats_async(struct cm_bearer_stats *self,
			  cm_bearer_get_stats_done done,
			  void *userdata);

struct cm_bearer_ip_config * cm_bearer_obj_get_ip_config(struct cm_bearer *self,
						     cm_err_t *err);

struct cm_bearer_ip_config *
cm_bearer_obj_get_ip_config_async(struct cm_bearer *self,
			  cm_bearer_get_ip_config_done done,
			  void *userdata);

const char * cm_bearer_obj_get_interface(struct cm_bearer *self,
				     cm_err_t *err);

const char * cm_bearer_obj_get_interface_async(struct cm_bearer *self,
					   cm_bearer_get_interface_done done,
					   void *userdata);

void cm_bearer_obj_subscribe_state_update(struct cm_bearer *self,
				      cm_bearer_stats_updated updated,
				      void *userdata,
				      cm_err_t *err);

void cm_bearer_obj_unsubscribe_state_update(struct cm_bearer *self,
				      cm_err_t *err);

void cm_bearer_obj_subscribe_stats_update(struct cm_bearer *self,
				      cm_bearer_stats_updated updated,
				      void *userdata,
				      cm_err_t *err);

void cm_bearer_obj_unsubscribe_stats_update(struct cm_bearer *self,
				      cm_err_t *err);

void cm_bearer_obj_subscribe_ip_config_update(struct cm_bearer *self,
					  cm_bearer_ip_config_updated updated,
					  void *userdata,
					  cm_err_t *err);

void cm_bearer_obj_unsubscribe_ip_config_update(struct cm_bearer *self,
					  cm_err_t *err);

void cm_bearer_obj_subscribe_interface_update(struct cm_bearer *self,
					  cm_bearer_interface_updated updated,
					  void *userdata,
					  cm_err_t *err);

void cm_bearer_obj_unsubscribe_ip_config_update(struct cm_bearer *self,
					  cm_err_t *err);


#endif /* _CM_BEARER_PRIV_H_ */
