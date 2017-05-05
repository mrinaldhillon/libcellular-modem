#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <semaphore.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_object.h"
#include "cm_module.h"
#include "cm_manager_priv.h"
#include "cm_manager_obj.h"
#include "cm_modem.h"

#define	CLASS_NAME		"CMManager"
/* @tbd: Should CMManeger be singleton */
//@todo: revisit unloading logic and semaphore logic
static sem_t mutex;

__attribute__((constructor)) void cm_manager_obj_constructor(void)
{
	sem_init(&mutex, 0, 1);
	cm_debug("Loading: %s", CLASS_NAME);
}
__attribute__((destructor)) void cm_manager_obj_destructor(void)
{
	sem_wait(&mutex);
	cm_debug("Unloading: %s", CLASS_NAME);
	sem_destroy(&mutex);
}

static const char * cm_manager_obj_get_class_name(void)
{
	return CLASS_NAME;
}

static void cm_manager_obj_start(struct cm_manager *self, cm_err_t *err)
{
	assert(self && self->priv && err);
	// start each manager in cmm set
}

static void cm_manager_obj_start_async(struct cm_manager *self,
			    cm_manager_start_done done,
			    void *userdata)
{
	assert(self && self->priv);
	// start each manager in cmm set
}

static void cm_manager_obj_stop(struct cm_manager *self, cm_err_t *err)
{
	assert(self && self->priv && err);
	// stop each manager in cmm set
}

static void cm_manager_obj_stop_async(struct cm_manager *self,
			   cm_manager_stop_done done,
			   void *userdata)
{
	assert(self && self->priv);
	// stop each manager in cmm set
}

static void cm_manager_obj_list_modems(struct cm_manager *self,
			    cm_manager_list_modems_for_each for_each,
			    void *userdata,
			    cm_err_t *err)
{
	assert(self && self->priv && err);
	// list from each manager
}

static void cm_manager_obj_list_modems_async(struct cm_manager *self,
				  cm_manager_list_modems_for_each for_each,
				  cm_manager_list_modems_done done,
				  void *userdata)
{
	assert(self && self->priv);
	// list from each manager
}

static void cm_manager_obj_subscribe_modem_added(struct cm_manager *self,
				      cm_manager_modem_added added,
				      void *userdata,
				      cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager

}

static void cm_manager_obj_unsubscribe_modem_added(struct cm_manager *self,
					cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

static void cm_manager_obj_subscribe_modem_removed(struct cm_manager *self,
					cm_manager_modem_removed removed,
					void *userdata,
					cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

static void cm_manager_obj_unsubscribe_modem_removed(struct cm_manager *self,
					  cm_err_t *err)
{
	assert(self && self->priv && err);
	// from each manager
}

static void cm_manager_obj_release_for_each_modem(struct cm_object *modemobj,
					     void *userdata)
{
	assert(modemobj);
	struct cm_manager *self = (struct cm_manager *)userdata;
	/* Deleting the modem from internal linked list
	 * since 'put' may not release it from the list
	 * if client may have held reference to this modem.
	 * Upon releasing last reference, will this modem
	 * be removed from memory, though by deleting from set cm-manager
	 * has stopped managing this modem. */
	cm_object_del(modemobj);
	cm_object_put(modemobj);
	cm_atomic_dec(&self->priv->num_modems);
}

static void cm_manager_obj_cleanup(struct cm_manager *self)
{
	assert(self && self->priv);
	cm_set_for_each_safe(self->priv->modems,
			     &cm_manager_obj_release_for_each_modem,
			     self);
	assert(0 == cm_atomic_read(&self->priv->num_modems));
	cm_debug("CMManager cleanup done");
	cm_set_del(self->priv->modems);
	cm_set_put(self->priv->modems);
}

static void cm_manager_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_manager *self = to_cm_manager(cmobj);
	if (self->priv->owner)
		cm_object_put(&self->priv->owner->cmobj);

	cm_debug("Destroying %s", cm_object_get_name(cmobj));
	free(self->priv);
	free(self);
	/* Increment value to allow destructor to proceed
	 * This dirty but required, cannot allow unloading from namespace while
	 * in memory.
	 * @todo figure alternate solution
	 */
	sem_post(&mutex);
}

struct cm_manager * cm_manager_obj_new(struct cm_module *owner,
				       cm_err_t *err)
{
	assert(err);
	struct cm_manager_priv *priv =
		(struct cm_manager_priv *)calloc(1, sizeof(*priv));
	if (NULL == priv) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	struct cm_manager *self =
		(struct cm_manager *)calloc(1, sizeof(*self));
	if (NULL == self) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	cm_object_init(&self->cmobj);
	self->cmobj.release = &cm_manager_obj_release;
	/* @todo: modems should be created in start */
	priv->modems = cm_set_create_and_add(&self->cmobj, NULL,
					     err, "CMModems");
	if (CM_ERR_NONE != *err) {
		cm_error("Error in creating CMModems cmset %d", *err);
		goto out_unref;
	}

	cm_atomic_set(&priv->num_modems, 0);
	if (owner)
		cm_object_get(&owner->cmobj);
	priv->owner = owner;
	self->priv = priv;

	self->cleanup = &cm_manager_obj_cleanup;
	self->start = &cm_manager_obj_start;
	self->start_async = &cm_manager_obj_start_async;
	self->stop = &cm_manager_obj_stop;
	self->stop_async = &cm_manager_obj_stop_async;
	self->list_modems = &cm_manager_obj_list_modems;
	self->list_modems_async = &cm_manager_obj_list_modems_async;
	self->get_class_name = &cm_manager_obj_get_class_name;

	if (owner) {
		/* decrement value to 0 in order to block destructor.
		 * library is unloaded upon relasing owner in release
		 */
		sem_wait(&mutex);
	}

	return self;
out_unref:
	cm_manager_obj_cleanup(self);
	cm_object_put(&self->cmobj);
	return NULL;
}

/* Dynamic library loading is done in two steps
 * 1. symbol = dlsym "cm_manager_obj_get_entry_symbol"
 * 2. struct cm_manager *manager = dlsym symbol
 * @todo 1 step should be sufficient. This is cm_module_loader api issue to keep
 * it independent of specific type to load.
 */
const char * cm_get_module_entry_symbol(void)
{
	return "cm_manager_obj_new";
}
