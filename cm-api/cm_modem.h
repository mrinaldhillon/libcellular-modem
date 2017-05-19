#ifndef _CM_MODEM_H_
#define _CM_MODEM_H_

#include "cm_err.h"
#include "cm_modem_state.h"
#include "cm_signal.h"
#include "cm_bearer.h"
#include "cm_bearer_properties.h"
#include "cm_modem_connect_properties.h"
#include "cm_sim.h"
#include "cm_messaging.h"
#include "cm_modem_clbk_defs.h"

struct cm_modem;

struct cm_modem * cm_modem_ref(struct cm_modem *self);

void cm_modem_unref(struct cm_modem *self);

/*
 * /CMManager/CMManagerInfaceName/Modem#
 */
char * cm_modem_get_path(struct cm_modem *self);

/*
 * More hardware properties need to be added
 */
const char * cm_modem_get_manufacturer(struct cm_modem *self);

const char * cm_modem_get_model(struct cm_modem *self);

const char * cm_modem_get_equipment_id(struct cm_modem *self);

unsigned int cm_modem_get_signal_quality(struct cm_modem *self);

void cm_modem_enable(struct cm_modem *self,
		     cm_err_t *err);

void cm_modem_enable_async(struct cm_modem *self,
			   cm_modem_enable_done done,
			   void *userdata);

void cm_modem_disable(struct cm_modem *self,
		      cm_err_t *err);

void cm_modem_disable_async(struct cm_modem *self,
			    cm_modem_disable_done done,
			    void *userdata);

cm_modem_state_t cm_modem_get_state(struct cm_modem *self,
				    cm_err_t *err);

struct cm_bearer * cm_modem_connect(struct cm_modem *self,
				    struct cm_modem_connect_properties *properties,
				    cm_err_t *err);

void cm_modem_connect_async(struct cm_modem *self,
			    struct cm_modem_connect_properties *properties,
			    cm_modem_connect_done done,
			    void *userdata);

void cm_modem_disconnect(struct cm_modem *self,
			 const char *bearer_path,
			 cm_err_t *err);

void cm_modem_disconnect_async(struct cm_modem *self,
			       const char *bearer_path,
			       cm_modem_connect_done done,
			       void *userdata);

struct cm_signal *cm_modem_get_signal(struct cm_modem *self,
				      cm_err_t *err);

void cm_modem_get_signal_async(struct cm_modem *self,
			       cm_modem_get_signal_done done,
			       void *userdata);

struct cm_bearer *
cm_modem_create_bearer(struct cm_modem *self,
		       struct cm_bearer_properties *properties,
		       cm_err_t *err);

void cm_modem_create_bearer_async(struct cm_modem *self,
				  struct cm_bearer_properties *properties,
				  cm_modem_create_bearer_done done,
				  void *userdata);

void cm_modem_delete_bearer(struct cm_modem *self,
			    const char *bearer_path,
			    cm_err_t *err);

void cm_modem_delete_bearer_async(struct cm_modem *self,
			    const char *bearer_path,
			    cm_modem_delete_bearer_done done,
			    void *userdata);

void cm_modem_list_bearers(struct cm_modem *self,
			   cm_modem_list_bearers_for_each for_each,
			   cm_modem_list_bearers_done done,
			   void *userdata,
			   cm_err_t *err);

void cm_modem_list_bearers_async(struct cm_modem *self,
				 cm_modem_list_bearers_for_each for_each,
				 cm_modem_list_bearers_done done,
				 void *userdata);

void cm_modem_subscribe_state_update(struct cm_modem *self,
				     cm_modem_state_updated updated,
				     void *userdata,
				     cm_err_t *err);

void cm_modem_unsubscribe_state_update(struct cm_modem *self,
				       cm_err_t *err);

void cm_modem_subscribe_signal_update(struct cm_modem *self,
				      cm_modem_signal_updated updated,
				      void *userdata,
				      cm_err_t *err);

void cm_modem_unsubscribe_signal_update(struct cm_modem *self,
					 cm_err_t *err);

#endif /* _CM_MODEM_H_ */
