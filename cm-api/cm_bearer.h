#ifndef _CM_BEARER_H_
#define _CM_BEARER_H_

#include "cm_bearer_state.h"
#include "cm_bearer_properties.h"
#include "cm_bearer_stats.h"
#include "cm_bearer_ip_config.h"

struct cm_bearer;

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

struct cm_bearer * cm_bearer_ref(struct cm_bearer *self);

void cm_bearer_unref(struct cm_bearer *self);

const char * cm_bearer_get_path(struct cm_bearer *self);

/*
 * Not sure if this is required, since parent path can be parsed
 * from the bearer path i.e. ../Modem#/Bearer#
 */
const char * cm_bearer_get_modem_path(struct cm_bearer *self);

unsigned int cm_bearer_is_connected(struct cm_bearer *self);

void cm_bearer_connect(struct cm_bearer *self, cm_err_t *err);

void cm_bearer_connect_async(struct cm_bearer *self,
			     cm_bearer_connect_done done,
			     void *userdata);

void cm_bearer_disconnect(struct cm_bearer *self, cm_err_t *err);

void cm_bearer_disconnect_async(struct cm_bearer *self,
			     cm_bearer_disconnect_done done,
			     void *userdata);

struct cm_bearer_properties * cm_bearer_get_properties(struct cm_bearer *self);

/*
 * @tbd:
 * For now stats are not local property and will be fetched from base manager
 * over ipc hence the err and asyc callback. Later we will implement an update
 * event handler per live object, that will allow the live object to locally
 * update its properties based on notifications. So property returned will be
 * most updated and will not require async call as well as error handling.
 */
struct cm_bearer_stats * cm_bearer_get_stats(struct cm_bearer_stats *self,
						 cm_err_t *err);
struct cm_bearer_stats *
cm_bearer_get_stats_async(struct cm_bearer_stats *self,
			  cm_bearer_get_stats_done done,
			  void *userdata);

struct cm_bearer_ip_config * cm_bearer_get_ip_config(struct cm_bearer *self,
						     cm_err_t *err);

struct cm_bearer_ip_config *
cm_bearer_get_ip_config_async(struct cm_bearer *self,
			  cm_bearer_get_ip_config_done done,
			  void *userdata);

const char * cm_bearer_get_interface(struct cm_bearer *self,
				     cm_err_t *err);

const char * cm_bearer_get_interface_async(struct cm_bearer *self,
					   cm_bearer_get_interface_done done,
					   void *userdata);

void cm_bearer_subscribe_state_update(struct cm_bearer *self,
				      cm_bearer_stats_updated updated,
				      void *userdata,
				      cm_err_t *err);

void cm_bearer_unsubscribe_state_update(struct cm_bearer *self,
				      cm_err_t *err);

void cm_bearer_subscribe_stats_update(struct cm_bearer *self,
				      cm_bearer_stats_updated updated,
				      void *userdata,
				      cm_err_t *err);

void cm_bearer_unsubscribe_stats_update(struct cm_bearer *self,
				      cm_err_t *err);

void cm_bearer_subscribe_ip_config_update(struct cm_bearer *self,
					  cm_bearer_ip_config_updated updated,
					  void *userdata,
					  cm_err_t *err);

void cm_bearer_unsubscribe_ip_config_update(struct cm_bearer *self,
					  cm_err_t *err);

void cm_bearer_subscribe_interface_update(struct cm_bearer *self,
					  cm_bearer_interface_updated updated,
					  void *userdata,
					  cm_err_t *err);

void cm_bearer_unsubscribe_ip_config_update(struct cm_bearer *self,
					  cm_err_t *err);

#endif /* _CM_BEARER_H_ */
