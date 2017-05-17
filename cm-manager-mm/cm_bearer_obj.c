#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "cm_log.h"
#include "cm_err.h"
#include "cm_object.h"
#include "cm_bearer_obj.h"
#include "cm_bearer_priv.h"
#include "cm_bearer_properties_obj.h"

void cm_bearer_obj_connect(struct cm_bearer *self, cm_err_t *err)
{
	assert(self && self->priv && self->priv->mm_bearer);
	GError *gerr = NULL;
	mm_bearer_connect_sync(self->priv->mm_bearer, NULL, &gerr);
	if (gerr) {
		cm_error("Error in CMBeare connect %s", gerr->message);
		*err = CM_ERR_BEARER_MM_CONNECT;
		g_error_free(gerr);
		return;
	}
	cm_info("Connected data bearer %s", self->get_path(self));
}

void cm_bearer_obj_disconnect(struct cm_bearer *self, cm_err_t *err)
{
	assert(self && self->priv && self->priv->mm_bearer);
	GError *gerr = NULL;
	mm_bearer_disconnect_sync(self->priv->mm_bearer, NULL, &gerr);
	if (gerr) {
		cm_error("Error in CMBeare disconnect %s", gerr->message);
		*err = CM_ERR_BEARER_MM_DISCONNECT;
		g_error_free(gerr);
		return;
	}
	cm_info("Dis-connected data bearer %s", self->get_path(self));
}

struct cm_bearer_ip_config * cm_bearer_obj_get_ip_config(struct cm_bearer *self,
						     cm_err_t *err)
{
	assert(self && err);
	GError *gerr = NULL;
	// @todo support more types
	MMBearerIpConfig * mm_ip_config =
		mm_bearer_get_ipv4_config(self->priv->mm_bearer);
	if (gerr) {
		cm_error("Error in fetching MMBearerIpConfig %s",
			 gerr->message);
		*err = CM_ERR_BEARER_MM_GET_IP_CONFIG;
		g_error_free(gerr);
		return NULL;
	}

	struct cm_bearer_ip_config * ip_config =
		cm_bearer_ip_config_obj_new(mm_ip_config);
	g_object_unref(mm_ip_config);
	return ip_config;
}

const char * cm_bearer_obj_get_interface(struct cm_bearer *self, cm_err_t *err)
{
	assert(self && err);
	// @todo support more types
	return (const char *)
		mm_bearer_get_interface(self->priv->mm_bearer);
}

struct cm_bearer_properties *
cm_bearer_obj_get_properties(struct cm_bearer *self)
{
	assert(self && self->priv && self->priv->properties);
	return self->priv->properties->get(self->priv->properties);
}

char * cm_bearer_obj_get_path(struct cm_bearer *self)
{
	assert(self);
	return cm_object_get_path(&self->cmobj);
}

static struct cm_bearer * cm_bearer_obj_get(struct cm_bearer *self)
{
	assert(self && self->priv);

	cm_object_get(&self->cmobj);
	return self;
}

static void cm_bearer_obj_put(struct cm_bearer *self)
{
	assert(self && self->priv);

	cm_object_put(&self->cmobj);
}

static void cm_bearer_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_bearer *self = to_cm_bearer(cmobj);

	cm_debug("Destroying %s", cm_object_get_name(cmobj));
	if (self->priv->mm_bearer)
		g_object_unref(self->priv->mm_bearer);
	if (self->priv->properties)
		self->priv->properties->put(self->priv->properties);

	free(self->priv);
	free(self);
}

struct cm_bearer * cm_bearer_new(struct cm_bearer_properties *properties,
				     cm_err_t *err)
{
	assert(properties && err);
	struct cm_bearer_priv *priv =
		(struct cm_bearer_priv *)calloc(1, sizeof(*priv));
	if (!priv) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	struct cm_bearer *self =
		(struct cm_bearer *)calloc(1, sizeof(*self));
	if (!self) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	cm_object_init(&self->cmobj);
	self->cmobj.release = &cm_bearer_obj_release;
	cm_object_set_name(&self->cmobj, CM_BEARER_CLASS_NAME);

	cm_atomic_set(&priv->state_deleted, 0);
	self->priv = priv;
	self->priv->properties = properties->get(properties);
	self->get = &cm_bearer_obj_get;
	self->put = &cm_bearer_obj_put;
	self->get_path = &cm_bearer_obj_get_path;
	self->connect = &cm_bearer_obj_connect;
	self->disconnect = &cm_bearer_obj_disconnect;
	self->get_ip_config = &cm_bearer_obj_get_ip_config;
	self->get_interface = &cm_bearer_obj_get_interface;

	return self;
}

void cm_bearer_obj_set_mm_bearer(struct cm_bearer *self, MMBearer *mm_bearer)
{
	assert(self && self->priv && mm_bearer);

	self->priv->mm_bearer = g_object_ref(mm_bearer);
}
