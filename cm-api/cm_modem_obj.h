#ifndef _CM_MODEM_OBJ_H_
#define _CM_MODEM_OBJ_H_

#include "cm_object.h"
#include "cm_err.h"
#include "cm_modem_state.h"
#include "cm_signal.h"
#include "cm_bearer_obj.h"
#include "cm_bearer_properties_obj.h"
#include "cm_modem_connect_properties_obj.h"
// @todo messaging and sim obj
#include "cm_sim.h"
#include "cm_messaging.h"
#include "cm_modem_clbk_defs.h"

struct cm_modem_priv;

struct cm_modem {
	struct cm_object cmobj;

	struct cm_modem_priv *priv;

	const char *(*get_class_name)(void);

	struct cm_modem *(*get)(struct cm_modem *self);

	void (*put)(struct cm_modem *self);

	/*
	 * /CMManager/CMManagerInfaceName/Modem#
	 */
	char *(*get_path)(struct cm_modem *self);

	/*
	 * More hardware properties need to be added
	 */
	const char *(*get_manufacturer)(struct cm_modem *self);

	const char *(*get_model)(struct cm_modem *self);

	const char *(*get_equipment_id)(struct cm_modem *self);

	unsigned int (*get_signal_quality)(struct cm_modem *self);

	void(*enable)(struct cm_modem *self,
		      cm_err_t *err);

	void(*enable_async)(struct cm_modem *self,
			    cm_modem_enable_done done,
			    void *userdata);

	void(*disable)(struct cm_modem *self,
		       cm_err_t *err);

	void(*disable_async)(struct cm_modem *self,
			     cm_modem_disable_done done,
			     void *userdata);

	cm_modem_state_t (*get_state)(struct cm_modem *self,
				      cm_err_t *err);

	void (*get_state_async)(struct cm_modem *self,
				cm_modem_get_state_done done,
				void *userdata);

	struct cm_bearer *(*connect)(struct cm_modem *self,
				     struct cm_modem_connect_properties
				     *properties,
				     cm_err_t *err);

	void (*connect_async)(struct cm_modem *self,
			      struct cm_modem_connect_properties *properties,
			      cm_modem_connect_done done,
			      void *userdata);

	void (*disconnect)(struct cm_modem *self,
			   const char *bearer_path,
			   cm_err_t *err);

	void (*disconnect_async)(struct cm_modem *self,
				 const char *bearer_path,
				 cm_modem_connect_done done,
				 void *userdata);

	struct cm_signal * (*get_signal)(struct cm_modem *self,
				       cm_err_t *err);

	struct cm_bearer *
		(*create_bearer)(struct cm_modem *self,
				 struct cm_bearer_properties *properties,
				 cm_err_t *err);

	void (*create_bearer_async)(struct cm_modem *self,
				    struct cm_bearer_properties *properties,
				    cm_modem_create_bearer_done done,
				    void *userdata);

	void (*delete_bearer)(struct cm_modem *self,
			      const char *bearer_path,
			      cm_err_t *err);

	void (*delete_bearer_async)(struct cm_modem *self,
			      const char *bearer_path,
			      cm_modem_delete_bearer_done done,
			      void *userdata);

	void (*list_bearers)(struct cm_modem *self,
			     cm_modem_list_bearers_for_each for_each,
			     cm_modem_list_bearers_done done,
			     void *userdata,
			     cm_err_t *err);

	void (*list_bearers_async)(struct cm_modem *self,
				   cm_modem_list_bearers_for_each for_each,
				   cm_modem_list_bearers_done done,
				   void *userdata);

	void (*subscribe_state_update)(struct cm_modem *self,
				       cm_modem_state_updated updated,
				       void *userdata,
				       cm_err_t *err);

	void (*unsubscribe_state_update)(struct cm_modem *self,
					 cm_err_t *err);

	void (*subscribe_signal_update)(struct cm_modem *self,
					cm_modem_signal_updated updated,
					void *userdata,
					cm_err_t *err);

	void (*unsubscribe_signal_update)(struct cm_modem *self,
					   cm_err_t *err);
};

static inline struct cm_modem * to_cm_modem(struct cm_object *cmobj)
{
	return cmobj ? cm_container_of(cmobj, struct cm_modem, cmobj) : NULL;
}

struct cm_modem * cm_modem_obj_new(cm_err_t *err);

void cm_modem_obj_new_async(cm_modem_new_done done,
			  void *userdata);

#endif /* _CM_MODEM_OBJ_H_ */
