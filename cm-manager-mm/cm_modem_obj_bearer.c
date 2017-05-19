#include <libmm-glib.h>
#include <assert.h>
#include "cm_log.h"
#include "cm_err.h"
#include "cm_object.h"
#include "cm_bearer_obj.h"
#include "cm_bearer_priv.h"
#include "cm_modem_obj.h"
#include "cm_modem_priv.h"
#include "cm_bearer_properties_obj.h"
#include "cm_bearer_properties_priv.h"

struct cm_bearer *
cm_modem_obj_create_bearer(struct cm_modem *self,
			   struct cm_bearer_properties *properties,
			   cm_err_t *err)
{
	assert(self && self->priv && self->priv->mm_modem &&
	       properties && properties->priv
	       && properties->priv->mm_bearer_properties && err);
	struct cm_bearer *bearer = NULL;
	GError *gerr = NULL;
	MMBearer *mm_bearer =
		mm_modem_create_bearer_sync(self->priv->mm_modem,
					    properties->priv->
					    mm_bearer_properties,
					    NULL, &gerr);
	if (gerr) {
		cm_error("Error in creating MMBearer %s", gerr->message);
		*err |= CM_ERR_MODEM_MM_CREATE_BEARER;
		g_error_free(gerr);
		return NULL;
	}

	bearer = cm_bearer_new(properties, err);
	cm_bearer_obj_set_mm_bearer(bearer, mm_bearer);

	bearer->get(bearer);
	cm_object_add(&bearer->cmobj, &self->cmobj, self->priv->bearers,
		      err, "%s-%d", CM_BEARER_CLASS_NAME,
		      cm_atomic_inc_and_read(&self->priv->num_bearers));

	g_object_unref(mm_bearer);
	return bearer;
}

void
cm_modem_obj_delete_bearer(struct cm_modem *self,
			   const char *bearer_path,
			   cm_err_t *err)
{
	assert(self && self->priv && self->priv->mm_modem && bearer_path);
	struct cm_bearer *bearer = NULL;
	struct cm_object *bearer_obj = cm_set_find_object(self->priv->bearers,
							  basename(bearer_path));
	if (!bearer_obj) {
		cm_warn("Could not find bearer %s", bearer_path);
		*err = CM_ERR_MODEM_MM_BEARER_NOT_FOUND;
		return;
	}
	bearer = to_cm_bearer(bearer_obj);

	GError *gerr = NULL;
	mm_modem_delete_bearer_sync(self->priv->mm_modem,
			mm_bearer_get_path(bearer->priv->mm_bearer),
					    NULL, &gerr);
	if (gerr) {
		cm_error("Error in deleting bearer %s", gerr->message);
		*err |= CM_ERR_MODEM_MM_DELETE_BEARER;
		g_error_free(gerr);
		goto out_put_bearerobj;
	}
	cm_atomic_set(&bearer->priv->state_deleted, 1);

out_put_bearerobj:
	cm_object_put(bearer_obj);
}
