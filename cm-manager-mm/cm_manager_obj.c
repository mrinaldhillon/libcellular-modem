#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <semaphore.h>
#include <libmm-glib.h>
#include "cm_thread.h"
#include "cm_err.h"
#include "cm_log.h"
#include "cm_object.h"
#include "cm_module.h"
#include "cm_manager_priv.h"
#include "cm_manager_obj.h"
#include "cm_modem_obj.h"
#include "cm_manager_mm_internal.h"

#define	CMMANGER_CLASS_NAME		"CMManager"
/* @tbd: Should CMManeger be singleton */
//@todo: revisit unloading logic and semaphore logic
static sem_t mutex;

__attribute__((constructor)) void cm_manager_obj_constructor(void)
{
	sem_init(&mutex, 0, 1);
	cm_debug("Loading: %s", CMMANGER_CLASS_NAME);
}
__attribute__((destructor)) void cm_manager_obj_destructor(void)
{
	sem_wait(&mutex);
	cm_debug("Unloading: %s", CMMANGER_CLASS_NAME);
	sem_destroy(&mutex);
}

static const char * cm_manager_obj_get_class_name(void)
{
	return CMMANGER_CLASS_NAME;
}

char * cm_manager_obj_get_path(struct cm_manager *self)
{
	assert(self);
	return cm_object_get_path(&self->cmobj);
}

static void cm_manager_obj_for_each_modem_get(struct cm_object *modemobj,
					      void *userdata)
{
	assert(modemobj);
	/* @todo: change this cm_modem_get once implemented in modem */
	struct cm_modem *modem = to_cm_modem(modemobj);
	modem->get(modem);
}

static struct cm_manager * cm_manager_obj_get(struct cm_manager *self)
{
	assert(self && self->priv);
	cm_object_get(&self->cmobj);
	cm_set_get(self->priv->modems);
	cm_set_for_each_safe(self->priv->modems,
			     &cm_manager_obj_for_each_modem_get,
			     self);
	// @todo is it really needed to be part of ref and unref
	// since following objects are not part of the hierarchy
	if (self->priv->modem_manager)
		g_object_ref(self->priv->modem_manager);
	if (self->priv->dbus_conn)
		g_object_ref(self->priv->dbus_conn);
	return self;
}

static void cm_manager_obj_for_each_modem_put(struct cm_object *modemobj,
					      void *userdata)
{
	assert(modemobj);
	/* @todo: change this cm_modem_get once implemented in modem */
	struct cm_modem *modem = to_cm_modem(modemobj);
	modem->put(modem);
}

void cm_manager_obj_put(struct cm_manager *self)
{
	assert(self && self->priv);
	cm_set_for_each_safe(self->priv->modems,
			     &cm_manager_obj_for_each_modem_put,
			     self);

	cm_set_put(self->priv->modems);
	if (self->priv->modem_manager)
		g_object_unref(self->priv->modem_manager);
	if (self->priv->dbus_conn)
		g_object_unref(self->priv->dbus_conn);
	cm_object_put(&self->cmobj);
}

#if 0
struct cm_manager_obj_put_ctx {
	struct cm_manager *cmm;
	struct cm_set *thread_ctxset;
	cm_err_t err;
};

static void
cm_manager_obj_for_modem_put_thread(struct cm_thread_ctx *thread_ctx)
{
	assert(thread_ctx);
	struct cm_modem *modem = (struct cm_modem *)thread_ctx->args;
	modem->put(modem);

	// balancing ref
	modem->put(modem);
}

static void cm_manager_obj_for_each_modem_put(struct cm_object *modemobj,
					      void *userdata)
{
	assert(modemobj);
	cm_err_t err = CM_ERR_NONE;
	struct cm_modem *modem = to_cm_modem(modemobj);
	struct cm_manager_obj_put_ctx *put_ctx =
		(struct cm_manager_obj_put_ctx *)userdata;
	assert(put_ctx && put_ctx->cmm && put_ctx->thread_ctxset);

	struct cm_thread_ctx *thread_ctx = cm_thread_ctx_new();
	// increase ref of object before pushing into thread ctx
	thread_ctx->args = modem->get(modem);
	thread_ctx->start_routine = &cm_manager_obj_for_modem_put_thread;
	cm_thread_ctx_add_and_create_joinable_thread(thread_ctx, NULL,
						     put_ctx->thread_ctxset,
						     &err);
	if (CM_ERR_NONE != err) {
		cm_warn("Could not create joinable thread %d", err);
		goto out_putmodem;
	}
	goto out_putthread_ctx;

out_putmodem:
	cm_warn("Error in put modem object in joinable thread, \
		doing serialized put");
	// balacing ref
	modem->put(modem);
	// actual put serialized since thread creation failed
	modem->put(modem);
out_putthread_ctx:
	cm_object_put(&thread_ctx->cmobj);
}

static void cm_manager_obj_put(struct cm_manager *self)
{
	assert(self && self->priv);
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager_obj_put_ctx *put_ctx =
		(struct cm_manager_obj_put_ctx *)calloc(1, sizeof(*put_ctx));
	if (!put_ctx) {
		cm_error("Unable to allocate enough memory %d", errno);
		abort();
	}
	put_ctx->cmm = self;
	put_ctx->thread_ctxset = cm_set_create();
	cm_object_set_name(&put_ctx->thread_ctxset->cmobj, "CMManagerThreads");

	cm_set_for_each_safe(self->priv->modems,
			     &cm_manager_obj_for_each_modem_put,
			     put_ctx);

	cm_thread_ctx_join_threads_and_del(put_ctx->thread_ctxset, &err);
	if (CM_ERR_NONE != err) {
		cm_warn("Could not join some/all threads to put modems, \
			this may result in memory leaks!!! %d", err);
	}

	cm_object_put(&put_ctx->thread_ctxset->cmobj);
	cm_set_put(self->priv->modems);
	if (self->priv->modem_manager)
		g_object_unref(self->priv->modem_manager);
	if (self->priv->dbus_conn)
		g_object_unref(self->priv->dbus_conn);
	cm_object_put(&self->cmobj);
	free(put_ctx);
}
#endif
static void cm_manager_obj_release(struct cm_object *cmobj)
{
	assert(cmobj);
	struct cm_manager *self = to_cm_manager(cmobj);
	struct cm_module *owner = self->priv->owner;
	cm_debug("Destroying %s", cm_object_get_name(cmobj));

	free(self->priv);
	free(self);
	if (owner) {
		/* decrement value to 0 in order to block destructor.
		 * library is unloaded upon relasing owner in release
		 */
		cm_debug("Releasing owner");
		sem_wait(&mutex);
		cm_object_put(&owner->cmobj);
		/* Increment value to allow destructor to proceed
		 * This dirty but required, cannot allow unloading from namespace while
		 * in memory.
		 * @todo figure alternate solution
		 */
		sem_post(&mutex);
	}
}

static void cm_manager_init_dbus_connection(struct cm_manager *self,
					    cm_err_t *err)
{
	assert(self && self->priv);
	GError *gerr = NULL;
	self->priv->dbus_conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &gerr);
	if (!self->priv->dbus_conn) {
		cm_error("Error in getting bus connection %s", gerr->message);
		*err |= CM_ERR_MANAGER_MM_GET_BUS;
	}
	if (gerr)
		g_error_free(gerr);
}

static void cm_manager_obj_modem_manager_new(struct cm_manager *self,
					     cm_err_t *err)
{
	assert(self && self->priv && self->priv->dbus_conn);
	GError *gerr = NULL;

	self->priv->modem_manager =
		mm_manager_new_sync(self->priv->dbus_conn,
				    G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_DO_NOT_AUTO_START,
				    NULL, &gerr);
	if (!self->priv->modem_manager) {
		cm_error("Error in creating ModemManager client %s",
			 gerr->message);
		*err |= CM_ERR_MANAGER_MM_CREATE_MODEM_MANAGER;
	}

	if (gerr)
		g_error_free(gerr);
}

static void cm_manager_obj_create_modem(struct cm_manager *self,
					MMObject *mmobj,
					cm_err_t *err)
{
	assert(mmobj);
	char *path = NULL;
	struct cm_modem *modem = cm_modem_obj_new(err);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in creating new CMModem object %d", *err);
		return;
	}

	cm_modem_obj_set_mm_modem_object(modem, mmobj, err);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in setting MMObject for CMModem %d", *err);
		goto out_putmodem;
	}

	cm_object_add(&modem->cmobj, &self->cmobj, self->priv->modems, err,
		      "%s-%d", modem->get_class_name(),
		      cm_atomic_inc_and_read(&self->priv->num_modems));

	path = modem->get_path(modem);
	cm_info("CMManager added modem %s", path);
	free(path);

	return;
out_putmodem:
	modem->put(modem);
}

static void cm_manager_obj_populate_modems(struct cm_manager *self,
					   cm_err_t *err)
{
	assert(self && self->priv && self->priv->modem_manager
	       && err);
	GList *mmobjs = NULL, *each = NULL;
	mmobjs =
	g_dbus_object_manager_get_objects(G_DBUS_OBJECT_MANAGER(self->priv->
								modem_manager));
	for (each = mmobjs; each; each = g_list_next(each)) {
		cm_manager_obj_create_modem(self, MM_OBJECT(each->data), err);
		if (CM_ERR_NONE != *err) {
			cm_error("Could not create CMModem object %d", *err);
			break;
		}
	}
	g_list_free_full(mmobjs, (GDestroyNotify) g_object_unref);;
}

//struct cm_manager_obj_subscribe_mm_manager_notifications(struct cm_manager *self,
//							 cm_err_t *err)

static void cm_manager_obj_init_modem_manager(struct cm_manager *self,
					      cm_err_t *err)
{
	assert(self && self->priv);

	cm_manager_init_dbus_connection(self, err);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in initializing dbus connection %d", *err);
		return;
	}

	cm_manager_obj_modem_manager_new(self, err);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in creating ModemManager %d", *err);
		goto out_dbusunref;
	}

	cm_manager_obj_populate_modems(self, err);

	//cm_manager_obj_subscribe_mm_manager_notofication(self, err);
	return;

out_dbusunref:
	g_object_unref(self->priv->dbus_conn);

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
	cm_object_set_name(&self->cmobj, CMMANGER_CLASS_NAME);
	self->cmobj.release = &cm_manager_obj_release;
	priv->modems = cm_set_create_and_add(&self->cmobj, NULL,
					     err, "CMModems");

	cm_atomic_set(&priv->num_modems, 0);
	self->priv = priv;
	self->get_class_name = &cm_manager_obj_get_class_name;
	self->get_path = &cm_manager_obj_get_path;
	self->get = &cm_manager_obj_get;
	self->put = &cm_manager_obj_put;
	self->list_modems = &cm_manager_obj_list_modems;
	self->list_modems_async = &cm_manager_obj_list_modems_async;


	cm_manager_obj_init_modem_manager(self, err);
	if (CM_ERR_NONE != *err) {
		cm_error("Error in initalizing ModemManager, \
			 destroying CMManager %d", *err);
		goto out_putself;
	}

	if (owner) {
		cm_object_get(&owner->cmobj);
		priv->owner = owner;
	}

	return self;
out_putself:
	self->put(self);
	return NULL;
}

//@todo need new and free methods for ctx
struct cm_manager_new_ctx {
	struct cm_module *owner;
	cm_manager_new_done done;
	void *userdata;
};

static void * cm_manager_obj_new_thread(void *userdata)
{
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager *self = NULL;
	struct cm_manager_new_ctx * ctx = NULL;

	ctx = (struct cm_manager_new_ctx *)userdata;
	assert(ctx && ctx->done);

	self = cm_manager_obj_new(ctx->owner, &err);
	ctx->done(self, ctx->userdata, err);
	/* remove the reference since callee may have take one
	 * else this object will be released*/
	if (self)
		self->put(self);
	free(ctx);
	return NULL;
}

void cm_manager_obj_new_async(struct cm_module *owner,
			      cm_manager_new_done done,
			      void *userdata)
{
	cm_err_t err = CM_ERR_NONE;

	assert(done);
	struct cm_manager_new_ctx *ctx =
		(struct cm_manager_new_ctx *)calloc(1, sizeof(*ctx));
	if (!ctx) {
		cm_error("Unable to allocate enough space %d",errno);
		abort();
	}

	ctx->owner = owner;
	ctx->done = done;
	ctx->userdata = userdata;

	cm_thread_t thread_id;
	cm_thread_create(&thread_id, &cm_manager_obj_new_thread,
			 ctx, CM_THREAD_CREATE_DETACHED, &err);
	if (CM_ERR_NONE != err) {
		err |= CM_ERR_MANAGER_NEW;
		goto out_freectx;
	}
	return;
out_freectx:
	free(ctx);
	done(NULL, userdata, err);
}

/* Dynamic library loading is done in two steps
 * 1. symbol = dlsym "cm_manager_obj_get_entry_symbol"
 * 2. struct cm_manager *manager = dlsym symbol
 * @todo 1 step should be sufficient. This is cm_module_loader api issue to keep
 * it independent of specific type to load.
 * @todo: add flag to return symbol for sync or async
 */
const char * cm_get_module_entry_symbol(void)
{
	return "cm_manager_obj_new";
}
