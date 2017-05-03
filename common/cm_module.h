#ifndef _CM_MODULE_H_
#define _CM_MODULE_H_

#include "cm_object.h"

#define CM_MODULE_STATIC_NAME		"CMModule"

struct cm_module {
	struct cm_object cmobj;
	void *handle;
	char *module_entry_symbol;
	void (*release)(void *handle);
};

struct cm_module * cm_module_create(void *module_handle,
				    const char *module_entry_symbol);

static inline struct cm_module * to_cm_module(struct cm_object *cmobj)
{
	return cmobj ? cm_container_of(cmobj, struct cm_module, cmobj) : NULL;
}

#endif /* _CM_MODULE_H_ */
