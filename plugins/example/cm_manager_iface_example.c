#include <semaphore.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_ref.h"
#include "cm_container_of.h"
#include "cm_manager_iface.h"
#include "cm_manager_iface_example_priv.h"

#define PLUGIN_NAME	"EXAMPLE-PLUGIN"

//@todo: revisit unloading logic
static sem_t mutex;

__attribute__((constructor)) void cm_manager_iface_constructor(void)
{
	sem_init(&mutex, 0, 1);
	cm_debug("Loading plugin: %s", PLUGIN_NAME);
}
__attribute__((destructor)) void cm_manager_iface_destructor(void)
{
	sem_wait(&mutex);
	cm_debug("Unloading plugin: %s", PLUGIN_NAME);
	sem_destroy(&mutex);
}

static void cm_manager_iface_release(struct cm_ref *ref)
{
	assert(NULL != ref);
	struct cm_manager_iface *self =
		cm_container_of(ref, struct cm_manager_iface, refcount);

	if (self->priv->notif) {
		self->priv->notif(self, self->priv->userdata);
	}
	free(self->priv);
	free(self);
	// Increment semaphore's value to allow destructor to proceed
	sem_post(&mutex);
}

static struct cm_manager_iface *
	cm_manager_iface_ref(struct cm_manager_iface *self)
{
	assert(NULL != self);
	cm_ref_get(&self->refcount);
	return self;
}

static void cm_manager_iface_unref(struct cm_manager_iface *self)
{
	assert(NULL != self);
	cm_ref_put(&self->refcount, &cm_manager_iface_release);
}

static void cm_manager_iface_set_notify_release(struct cm_manager_iface *self,
				   void (*notify)(struct cm_manager_iface *self,
						  void *userdata),
				   void *userdata)
{
	assert(NULL != self && NULL != notify && NULL != self->priv);
	if (self->priv->notif) {
		cm_warn("release notification can't be overridden\n");
		return;
	}
	self->priv->notif = notify;
	self->priv->userdata = userdata;
}

static const char * cm_manager_iface_get_name(struct cm_manager_iface *iface)
{
	return PLUGIN_NAME;
}

struct cm_manager_iface * cm_manager_iface_new(void)
{
	struct cm_manager_iface_priv *priv =
		(struct cm_manager_iface_priv *)calloc(1, sizeof(*priv));
	if (NULL == priv) {
		cm_error("Not enough space %d",errno);
		abort();
	}
	struct cm_manager_iface *self =
		(struct cm_manager_iface *)calloc(1, sizeof(*self));
	if (NULL == self) {
		cm_error("Not enough space %d",errno);
		abort();
	}

	cm_ref_init(&self->refcount);
	self->priv = priv;
	self->ref = &cm_manager_iface_ref;
	self->unref = &cm_manager_iface_unref;
	self->get_name = &cm_manager_iface_get_name;
	self->set_notify_release = &cm_manager_iface_set_notify_release;

	//decrement semphore's value if successfully created
	sem_wait(&mutex);
	return self;
}
