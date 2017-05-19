#include <stdlib.h>
#include <libmm-glib.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_modem_obj.h"
#include "cm_modem_priv.h"
#include "cm_modem_connect_properties_priv.h"
#include "cm_bearer_properties_priv.h"
#include "cm_bearer_properties_obj.h"
#include "cm_bearer_obj.h"
#include "cm_bearer_priv.h"

struct cm_bearer * cm_modem_obj_create_bearer_internal(MMBearer *mm_bearer,
						       cm_err_t *err)
{
	assert(mm_bearer);
	struct cm_bearer_properties *properties = NULL;
	struct cm_bearer *bearer = NULL;
	MMBearerProperties * mm_bearer_properties =
		mm_bearer_get_properties(mm_bearer);

	properties = cm_bearer_properties_obj_new_internal(mm_bearer_properties);
	bearer = cm_bearer_new(properties, err);
	cm_bearer_obj_set_mm_bearer(bearer, mm_bearer);
	g_object_unref(mm_bearer_properties);
	properties->put(properties);
	return bearer;
}

struct cm_bearer *cm_modem_obj_connect(struct cm_modem *self,
				       struct cm_modem_connect_properties *properties,
				       cm_err_t *err)
{
	assert(self && self->priv && self->priv->mm_simple_modem
	       && properties && err);
	struct cm_bearer *bearer = NULL;
	GError *gerr = NULL;
	MMBearer *mm_bearer =
		mm_modem_simple_connect_sync(self->priv->mm_simple_modem,
					     properties->
					     priv->mm_simple_connect_properties,
					     NULL, &gerr);
	if (gerr) {
		cm_error("Error in connecting the modem %s", gerr->message);
		*err = CM_ERR_MODEM_MM_CONNECT;
		g_error_free(gerr);
		return NULL;
	}
	cm_info("CMModem connected");
	bearer = cm_modem_obj_create_bearer_internal(mm_bearer, err);
	cm_object_add(&bearer->cmobj, &self->cmobj, self->priv->bearers,
		      err, "%s-%d", CM_BEARER_CLASS_NAME,
		      cm_atomic_inc_and_read(&self->priv->num_bearers));

	g_object_unref(mm_bearer);
	return bearer;
}

void cm_modem_obj_disconnect(struct cm_modem *self,
			     const char *bearer_path,
				       cm_err_t *err)
{
	assert(self && self->priv && self->priv->mm_simple_modem
	       && bearer_path && bearer_path[0] && err);
	GError *gerr = NULL;
	struct cm_bearer *bearer = NULL;
	struct cm_object *bearer_obj = cm_set_find_object(self->priv->bearers,
							  basename(bearer_path));
	if (!bearer_obj) {
		cm_warn("Could not find bearer %s", bearer_path);
		*err = CM_ERR_MODEM_MM_BEARER_NOT_FOUND;
		return;
	}
	bearer = to_cm_bearer(bearer_obj);

	mm_modem_simple_disconnect_sync(self->priv->mm_simple_modem,
			mm_bearer_get_path(bearer->priv->mm_bearer),
					     NULL, &gerr);
	if (gerr) {
		cm_error("Error in dis-connecting the modem %s", gerr->message);
		*err = CM_ERR_MODEM_MM_DISCONNECT;
		g_error_free(gerr);
		goto out_put_bearerobj;
	}
	cm_info("CMModem dis-connected");
out_put_bearerobj:
	cm_object_put(bearer_obj);
}
