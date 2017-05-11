#ifndef _CM_BEARER_OBJ_H_
#define _CM_BEARER_OBJ_H_

#include "cm_object.h"
#include "cm_err.h"
#include "cm_bearer_state.h"
#include "cm_bearer_stats_obj.h"
#include "cm_bearer_ip_config_obj.h"
#include "cm_bearer_properties_obj.h"
#include "cm_bearer_clbk_defs.h"

#define CM_BEARER_CLASS_NAME		"CMBearer"

struct cm_bearer_priv;

struct cm_bearer {
	struct cm_object cmobj;

	struct cm_bearer_priv *priv;

	const char * (*get_class_name)(void);

	char * (*get_path)(struct cm_bearer *self);

	struct cm_bearer * (*get)(struct cm_bearer *self);

	void (*put)(struct cm_bearer *self);

	char * (*get_modem_path)(struct cm_bearer *self);

	unsigned int (*is_connected)(struct cm_bearer *self);

	void (*connect)(struct cm_bearer *self, cm_err_t *err);

	void (*connect_async)(struct cm_bearer *self,
			      cm_bearer_connect_done done,
			      void *userdata);

	void (*disconnect)(struct cm_bearer *self, cm_err_t *err);

	void (*disconnect_async)(struct cm_bearer *self,
				 cm_bearer_disconnect_done done,
				 void *userdata);

	struct cm_bearer_properties *
		(*get_properties)(struct cm_bearer *self);

	struct cm_bearer_stats * (*get_stats)(struct cm_bearer *self,
					      cm_err_t *err);
	struct cm_bearer_stats *
		(*get_stats_async)(struct cm_bearer *self,
				   cm_bearer_get_stats_done done,
				   void *userdata);

	struct cm_bearer_ip_config * (*get_ip_config)(struct cm_bearer *self,
						      cm_err_t *err);

	struct cm_bearer_ip_config *
		(*get_ip_config_async)(struct cm_bearer *self,
				       cm_bearer_get_ip_config_done done,
				       void *userdata);

	const char * (*get_interface)(struct cm_bearer *self,
				      cm_err_t *err);

	const char *
		(*get_interface_async)(struct cm_bearer *self,
				       cm_bearer_get_interface_done done,
				       void *userdata);

	void (*subscribe_state_update)(struct cm_bearer *self,
				       cm_bearer_stats_updated updated,
				       void *userdata,
				       cm_err_t *err);

	void (*unsubscribe_state_update)(struct cm_bearer *self,
					 cm_err_t *err);

	void (*subscribe_stats_update)(struct cm_bearer *self,
				       cm_bearer_stats_updated updated,
				       void *userdata,
				       cm_err_t *err);

	void (*unsubscribe_stats_update)(struct cm_bearer *self,
					 cm_err_t *err);

	void (*subscribe_ip_config_update)(struct cm_bearer *self,
					   cm_bearer_ip_config_updated updated,
					   void *userdata,
					   cm_err_t *err);

	void (*unsubscribe_ip_config_update)(struct cm_bearer *self,
					     cm_err_t *err);

	void (*subscribe_interface_update)(struct cm_bearer *self,
					   cm_bearer_interface_updated updated,
					   void *userdata,
					   cm_err_t *err);

	void (*unsubscribe_interface_update)(struct cm_bearer *self,
					     cm_err_t *err);
};

static inline struct cm_bearer * to_cm_bearer(struct cm_object *cmobj)
{
	return cmobj ? cm_container_of(cmobj, struct cm_bearer, cmobj) : NULL;
}

struct cm_bearer * cm_bearer_new(struct cm_bearer_properties * properties,
				 cm_err_t *err);

struct cm_bearer * cm_bearer_new_async(cm_bearer_new_done done,
				       void *userdata);

#endif /* _CM_BEARER_OBJ_H_ */
