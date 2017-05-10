#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "cm_container_of.h"
#include "cm_log.h"
#include "cm_object.h"
#include "cm_list.h"
#include "cm_err.h"

/* Based on linux kobject */

static void cm_object_release(struct cm_ref *ref)
{
	assert(ref);
	struct cm_object *cmobj = cm_container_of(ref, struct cm_object, ref);
	if (cmobj->state_added)
		cm_object_del(cmobj);
	if (cmobj->release)
		cmobj->release(cmobj);
	cmobj->state_initialized = 0;
}

static void cm_set_release(struct cm_object *cmobj)
{
	struct cm_set *cmset = cm_container_of(cmobj, struct cm_set, cmobj);
	pthread_mutex_destroy(&cmset->lock);
	free(cmset);
}

static void cm_object_set_name_vargs(struct cm_object *self, const char *fmt,
				     va_list vargs)
{
	assert(self);
	if (self->name[0]) {
		cm_debug("cmobj:%s resetting name", self->name);
		memset(self->name, 0, CM_OBJECT_MAX_NAME_LEN);
	}

	vsnprintf(self->name, CM_OBJECT_MAX_NAME_LEN - 1, fmt, vargs);
	cm_debug("cmobj:%s", self->name);
}

void cm_object_set_name(struct cm_object *self, const char *fmt, ...)
{
	assert(self);

	va_list va;
	va_start(va, fmt);
	cm_object_set_name_vargs(self, fmt, va);
	va_end(va);
}

static void cm_object_init_internal(struct cm_object *cmobj)
{
	assert(cmobj);
	cm_ref_init(&cmobj->ref);
	cm_list_node_init(&cmobj->entry);
	cmobj->state_initialized = 1;
}

void cm_object_init(struct cm_object *self)
{
	assert(self && !self->state_initialized);
	memset(self, 0, sizeof(*self));
	cm_object_init_internal(self);
}

static void cm_object_join_cm_set(struct cm_object *cmobj)
{
	if (!cmobj->cmset)
		return;

	cm_set_get(cmobj->cmset);
	pthread_mutex_lock(&cmobj->cmset->lock);
	cm_list_add_tail(&cmobj->cmset->list, &cmobj->entry);
	pthread_mutex_unlock(&cmobj->cmset->lock);
}

static void cm_object_add_vargs(struct cm_object *self,
				struct cm_object *parent,
				struct cm_set *cmset,
				cm_err_t *err, const char *name_fmt,
				va_list vargs)
{
	assert(self && err && 1 == self->state_initialized);

	cm_object_set_name_vargs(self, name_fmt, vargs);

	if (!self->parent && parent)
		self->parent = cm_object_get(parent);
	// if !parent && cmobj->cmset then cmset is parent of cmobj
	if (cmset) {
		if (!self->parent)
			self->parent = cm_object_get(&cmset->cmobj);
		self->cmset = cmset;
		cm_object_join_cm_set(self);
	}
	self->state_added = 1;
}

void cm_object_add(struct cm_object *self, struct cm_object *parent,
		   struct cm_set *cmset, cm_err_t *err,
		   const char *name_fmt, ...)
{
	assert(self && err);
	va_list va;
	va_start(va, name_fmt);
	cm_object_add_vargs(self, parent, cmset, err, name_fmt, va);
	va_end(va);
}

void cm_object_init_and_add(struct cm_object *self, struct cm_object *parent,
			    struct cm_set *cmset, cm_err_t *err,
			    const char *name_fmt, ...)
{
	cm_object_init(self);
	va_list va;
	va_start(va, name_fmt);
	cm_object_add_vargs(self, parent, cmset, err, name_fmt, va);
	va_end(va);
}

static void cm_object_leave_cm_set(struct cm_object *cmobj)
{
	if (!cmobj->cmset)
		return;
	pthread_mutex_lock(&cmobj->cmset->lock);
	cm_list_del_init(&cmobj->entry);
	pthread_mutex_unlock(&cmobj->cmset->lock);
	cm_set_put(cmobj->cmset);
}

void cm_object_del(struct cm_object *self)
{
	if (!self)
		return;

	cm_object_leave_cm_set(self);
	if (self->parent)
		cm_object_put(self->parent);
	self->parent = NULL;
	self->state_added = 0;
}

struct cm_object * cm_object_get(struct cm_object *self)
{
	assert(self && 1 == self->state_initialized);
	// @tbd: should get always be get unless zero
	// also wouldn't it be programmer error if trying to take ref on object
	// with zero ref. Hence should assert on refcount > 0
	cm_ref_get(&self->ref);
	return self;
}

struct cm_object * cm_object_get_unless_zero(struct cm_object *self)
{
	assert(self && 1 == self->state_initialized);
	return 1 == cm_ref_get_not_zero(&self->ref) ? self : NULL;
}

void cm_object_put(struct cm_object *self)
{
	assert(self && 1 == self->state_initialized);
	cm_ref_put(&self->ref, &cm_object_release);
}

static int cm_object_get_path_len(struct cm_object *cmobj)
{
	int len = 1;
	struct cm_object *parent = cmobj;

	do {
		if (!cm_object_get_name(parent))
			return 0;
		len += strlen(cm_object_get_name(parent)) + 1;
		parent = parent->parent;
	} while(parent);
	cm_debug("cm_object: %s path len = %d", cm_object_get_name(cmobj), len);
	return len;
}

static void cm_object_fill_path(struct cm_object *cmobj, char *path, int len)
{
	struct cm_object *parent = NULL;
	--len;
	for (parent = cmobj; parent; parent = parent->parent) {
		int cur = strlen(cm_object_get_name(parent));
		len -= cur;
		strncpy(path + len, cm_object_get_name(parent), cur);
		*(path + --len) = '/';
	}
	cm_debug("cm_object: %s path = %s, len = %ld", cm_object_get_name(cmobj),
		 path, strlen(path));
}

char * cm_object_get_path(struct cm_object *self)
{
	assert(self);
	int len = 0;
	char *path = NULL;

	len = cm_object_get_path_len(self);
	path = (char *)calloc(len, sizeof(char));
	if (!path) {
		cm_error("Unable to allocate enough memory %d", errno);
		abort();
	}

	cm_object_fill_path(self, path, len);
	return path;
}

static void cm_set_init(struct cm_set *self)
{
	assert(self);
	cm_object_init_internal(&self->cmobj);
	cm_list_head_init(&self->list);
	pthread_mutex_init(&self->lock, NULL);
}

struct cm_set * cm_set_create(void)
{
	struct cm_set *self = (struct cm_set *)calloc(1, sizeof(*self));
	if (!self) {
		cm_error("Unable allocate enough memory %d", errno);
		abort();
	}
	cm_set_init(self);
	self->cmobj.release = &cm_set_release;
	return self;
}

void cm_set_add(struct cm_set *self, struct cm_object *parent,
		struct cm_set *cmset, cm_err_t *err, const char *name_fmt, ...)
{
	va_list va;
	va_start(va, name_fmt);
	cm_object_add_vargs(&self->cmobj, parent, cmset, err, name_fmt, va);
	va_end(va);
}

struct cm_set * cm_set_create_and_add(struct cm_object *parent,
				      struct cm_set *cmset, cm_err_t *err,
				      const char *name_fmt, ...)
{
	struct cm_set *self = cm_set_create();
	va_list va;
	va_start(va, name_fmt);
	cm_object_add_vargs(&self->cmobj, parent, cmset, err, name_fmt, va);
	va_end(va);
	return self;
}

void cm_set_del(struct cm_set *self)
{
	cm_object_del(&self->cmobj);
}

struct cm_set * cm_set_get(struct cm_set *self)
{
	assert(self);
	return self ? to_cm_set(cm_object_get(&self->cmobj)) : NULL;
}

void cm_set_put(struct cm_set *self)
{
	assert(self);
		cm_object_put(&self->cmobj);
}

struct cm_object * cm_set_get_next(struct cm_set *self,
				   struct cm_object *cur_entry)
{
	assert(self);
	if (!cur_entry)
		return NULL;
	struct cm_object *next_entry = NULL;
	pthread_mutex_lock(&self->lock);
	next_entry = cm_list_next(&self->list, cur_entry, entry);
	if (next_entry)
		cm_object_get(next_entry);
	pthread_mutex_unlock(&self->lock);
	return next_entry;
}

struct cm_object * cm_set_get_first(struct cm_set *self)
{
	assert(self);
	struct cm_object *first = NULL;
	pthread_mutex_lock(&self->lock);
	first = cm_list_top(&self->list, struct cm_object, entry);
	if (first)
		cm_object_get(first);
	pthread_mutex_unlock(&self->lock);
	return first;
}

void cm_set_for_each(struct cm_set *self,
		     void (*for_each)(struct cm_object *cmobj,
				      void *userdata),
		     void *userdata)
{
	assert(self && for_each);
	struct cm_object *cmobj = NULL;

	for(cmobj = cm_set_get_first(self); cmobj;
	    cmobj = cm_set_get_next(self, cmobj)) {
		for_each(cmobj, userdata);
		cm_object_put(cmobj);
	}
}

void cm_set_for_each_safe(struct cm_set *self,
			  void (*for_each)(struct cm_object *cmobj,
					   void *userdata),
			  void *userdata)
{
	assert(self && for_each);
	struct cm_object *cmobj = NULL, *next = NULL;
	// @todo revisit
	for(cmobj = cm_set_get_first(self), next = cm_set_get_next(self, cmobj);
	    cmobj; next = cm_set_get_next(self, cmobj)) {
		for_each(cmobj, userdata);
		cm_object_put(cmobj);
		if (!next) break;
		cmobj = next;
	}
}

struct cm_object * cm_set_find_object(struct cm_set *self, const char *name)
{
	assert(self && name);
	struct cm_object *cmobj = NULL, *cmobj_entry = NULL;
	pthread_mutex_lock(&self->lock);
	cm_list_for_each(&self->list, cmobj_entry, entry) {
		if (!strcmp(cm_object_get_name(cmobj_entry), name)) {
			cmobj = cm_object_get_unless_zero(cmobj_entry);
			break;
		}
	}
	pthread_mutex_unlock(&self->lock);
	return cmobj;
}
