#ifndef _CM_OBJECT_H_
#define _CM_OBJECT_H_

#include "cm_err.h"
#include "cm_ref.h"
#include "cm_list.h"
#include "cm_container_of.h"

#define CM_OBJECT_MAX_NAME_LEN	1024


struct cm_object {
	char name[1024];
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
	unsigned int state_added:1;
};

/* @todo provide vargs input parameters for setting name
 * i.e. cm_object_set_name(obj, "myobj", uid, myobj_index, ...);
 * */
void cm_object_set_name(struct cm_object *self, const char *fmt, ...);
//void cm_object_set_name_vargs(struct cm_object *self, const char *fmt,
//			      va_list vargs);

static inline const char * cm_object_get_name(const struct cm_object *self)
{
	assert(self);
	return self->name;
}

void cm_object_init(struct cm_object *self);

/*
 * Add cm_object to under the parent hierarchy and cmset collection.
 */
void cm_object_add(struct cm_object *self, struct cm_object *parent,
		   struct cm_set *cmset, cm_err_t *err,
		   const char *name_fmt, ...);

void cm_object_init_and_add(struct cm_object *self, struct cm_object *parent,
			    struct cm_set *cmset, cm_err_t *err,
			    const char *name_fmt, ...);
/*
 * Remove cm_object from the parent hierarchy and cmset collection.
 */
void cm_object_del(struct cm_object *cmboj);

struct cm_object * cm_object_get(struct cm_object *self);

struct cm_object * cm_object_get_unless_zero(struct cm_object *self);

void cm_object_put(struct cm_object *self);

char * cm_object_get_path(struct cm_object *self);

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

static inline struct cm_set * to_cm_set(struct cm_object *cmobj)
{
	return cmobj ? cm_container_of(cmobj, struct cm_set, cmobj) : NULL;
}

//void cm_set_init(struct cm_set *cmset);

/*int cm_set_register(struct cm_set *cmset);

void cm_set_unregister(struct cm_set *cmset);
*/
//void cm_set_set_name(struct cm_set *cmset, const char *name);

struct cm_set * cm_set_create(const char *name);

void cm_set_add(struct cm_set *self, struct cm_object *parent,
		   struct cm_set *cmset, cm_err_t *err);

struct cm_set * cm_set_create_and_add(struct cm_object *parent,
				      struct cm_set *cmset, const char *name,
				      cm_err_t *err);

void cm_set_del(struct cm_set *self);

struct cm_set * cm_set_get(struct cm_set *self);

void cm_set_put(struct cm_set *self);

struct cm_object * cm_set_get_first(struct cm_set *self);

struct cm_object * cm_set_get_next(struct cm_set *self,
				   struct cm_object *cur_entry);

struct cm_object * cm_set_find_object(struct cm_set *self, const char *name);

void cm_set_for_each(struct cm_set *self,
		     void (*for_each)(struct cm_object *cmobj,
				      void *userdata),
		     void *userdata);

/* Must be used when deleting objects from a set */
void cm_set_for_each_safe(struct cm_set *self,
		     void (*for_each)(struct cm_object *cmobj,
				      void *userdata),
		     void *userdata);

#endif /* _CM_OBJECT_H_ */
