#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include "cm_container_of.h"
#include "cm_log.h"
#include "cm_object.h"
#include "cm_list.h"

/* Shameless copy of linux kobject */

static void cm_object_release(struct cm_ref *ref)
{
	assert(ref);
	struct cm_object *cmobj = cm_container_of(ref, struct cm_object, ref);

	cm_object_del(cmobj);
	if (cmobj->release)
		cmobj->release(cmobj);
	if (cmobj->name)
		free(cmobj->name);
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

static void cm_object_leave_cm_set(struct cm_object *cmobj)
{
	if (!cmobj->cmset)
		return;
	pthread_mutex_lock(&cmobj->cmset->lock);
	cm_list_del_init(&cmobj->entry);
	pthread_mutex_unlock(&cmobj->cmset->lock);
	cm_set_put(cmobj->cmset);
}

void cm_object_del(struct cm_object *cmobj)
{
	if (!cmobj)
		return;

	cm_object_leave_cm_set(cmobj);
	cm_object_put(cmobj->parent);
	cmobj->parent = NULL;
}

void cm_object_init(struct cm_object *cmobj)
{
	assert(cmobj && !cmobj->state_initialized);

	memset(cmobj, 0, sizeof(*cmobj));
	cm_ref_init(&cmobj->ref);
	cm_list_node_init(&cmobj->entry);
	cmobj->state_initialized = 1;
}

struct cm_object * cm_object_get(struct cm_object *cmobj)
{
	assert(cmobj && 1 == cmobj->state_initialized);
	cm_ref_get(&cmobj->ref);
	return cmobj;
}

void cm_object_put(struct cm_object *cmobj)
{
	assert(cmobj);
	cm_ref_put(&cmobj->ref, &cm_object_release);
}


int cm_object_add(struct cm_object *cmobj, struct cm_object *parent,
			 const char *name)
{
	assert(cmobj && (cmobj->name || (name && name[0])));

	if (1 != cmobj->state_initialized) {
		cm_error("Cannot add un-initialized cm_obejct");
		return -EINVAL;
	}
	// @todo revisit
	if (cmobj->name) {
		cmobj->name = strndup(name, strlen(name));
		if (!cmobj->name) {
			cm_error("Unable to allocate enough memory %d", errno);
			abort();
		}
	}

	if (parent)
		cmobj->parent = cm_object_get(parent);
	// if !parent && cmobj->cmset then cmset is parent of cmobj
	if (cmobj->cmset) {
		if (!parent)
			cmobj->parent = cm_object_get(&cmobj->cmset->cmobj);
		// add to cm_object to cmset
		cm_object_join_cm_set(cmobj);
	}

	return 0;
}

int cm_object_init_and_add(struct cm_object *cmobj, struct cm_object *parent,
			   const char *name)
{
	cm_object_init(cmobj);
	return cm_object_add(cmobj, parent, name);
}

struct cm_set * cm_set_new(const char *name, struct cm_object *parent)
{
	assert(name && name[0]);

	struct cm_set * cmset = (struct cm_set *)calloc(1, sizeof(*cmset));
	if (!cmset) {
		cm_warn("Unable allocate enough memory %d", errno);
		abort();
	}
	cm_object_init_and_add(&cmset->cmobj, parent, name);
	cm_list_head_init(&cmset->list);
	pthread_mutex_init(&cmset->lock, NULL);
}

static int cm_object_get_path_len(struct cm_object *cmobj)
{
	int len = 0;
	struct cm_object *parent = cmobj;

	do {
		if (!cm_object_get_name(parent))
			return 0;
		len += strlen(cm_object_get_name(parent)) + 1;
		parent = parent->parent;
	} while(parent);
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
	cm_debug("cm_object: %s path = %s\n", cm_object_get_name(cmobj), path);
}

char * cm_object_get_path(struct cm_object *cmobj)
{
	assert(cmobj);
	int len = 0;
	char *path = NULL;

	len = cm_object_get_path_len(cmobj);
	path = (char *)calloc(len, sizeof(char));
	if (!path)
		return NULL;
	cm_object_fill_path(cmobj, path, len);
	return path;
}
