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
	if (CM_ERR_NONE != *err) {
		cm_error("Error in creating new CMBearer object %d", *err);
		goto out_put_mmbearer;
	}

	cm_bearer_obj_set_mm_bearer(bearer, mm_bearer);
	g_object_unref(mm_bearer);

	bearer->get(bearer);
	cm_object_add(&bearer->cmobj, &self->cmobj, self->priv->bearers,
		      err, "%s-%d", CM_BEARER_CLASS_NAME,
		      cm_atomic_inc_and_read(&self->priv->num_bearers));

	return bearer;

out_put_mmbearer:
	g_object_unref(mm_bearer);
	return NULL;
}
