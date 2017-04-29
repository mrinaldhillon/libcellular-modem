#ifndef _CM_OBJECT_H_
#define _CM_OBJECT_H_

#include "cm_ref.h"
#include "cm_list.h"

struct cm_object {
	char *name;
	// Node is added to cm_set if cm_set exists
	struct cm_list_node entry;
	// Parent object provides inheritance, path resolution
	// and binds parent's life time to this child.
	struct cm_object *parent;
	// Also points to linked list of this type in parent.
	struct cm_set *cmset;
	// Refernce of this object
	struct cm_ref ref;
	// release function is deconstructor for the encapsulating
	// class of this object.
	void (*release)(struct cm_object *self);

	unsigned int state_initialized:1;
};

/* @todo provide vargs input parameters for setting name
 * i.e. cm_object_set_name(obj, "myobj", uid, myobj_index, ...);
 * */
int cm_object_set_name(struct cm_object *cmobj, const char *name);

static inline const char * cm_object_get_name(const struct cm_object *cmobj)
{
	assert(cmobj);
	return cmobj->name;
}

void cm_object_init(struct cm_object *cmobj);

int cm_object_add(struct cm_object *cmobj, struct cm_object *parent,
		  const char *name);

int cm_object_init_and_add(struct cm_object *cmobj, struct cm_object *parent,
			   const char *name);

void cm_object_del(struct cm_object *cmboj);

struct cm_object * cm_object_get(struct cm_object *cmobj);

void cm_object_put(struct cm_object *cmobj);

char * cm_object_get_path(struct cm_object *cmobj);

/* @tdb: linux also implements ktype and uevent mechanism */

/* cm_set is tighly coupled with cm_object
 * hence keeping in same file */

struct cm_set {
	// head of the list
	struct cm_list_head list;
	// Lock to access the embedded list
	pthread_mutex_t lock;
	// Object of this set
	struct cm_object cmobj;
};

void cm_set_init(struct cm_set *cmset);

int cm_set_register(struct cm_set *cmset);

void cm_set_unregister(struct cm_set *cmset);

void cm_set_set_name(struct cm_set *cmset, const char *name);

struct cm_set * cm_set_new(const char *name,
			   struct cm_object *parent);

static inline struct cm_set *to_cm_set(struct cm_object *cmobj)
{
	assert(cmobj);
	return cmobj ? cm_container_of(cmobj, struct cm_set, cmobj) : NULL;
}

static inline struct cm_set * cm_set_get(struct cm_set *cmset)
{
	assert(cmset);
	return cmset ? to_cm_set(cm_object_get(&cmset->cmobj)) : NULL;
}

static inline void cm_set_put(struct cm_set * cmset)
{
	assert(cmset);
	cm_object_put(&cmset->cmobj);
}

struct cm_set * cm_set_new(const char *name, struct cm_object *parent);

#endif /* _CM_OBJECT_H_ */
