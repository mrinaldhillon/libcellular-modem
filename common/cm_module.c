#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_object.h"
#include "cm_module.h"

void cm_module_release(struct cm_object *moduleobj)
{
	assert(moduleobj);
	struct cm_module *self = to_cm_module(moduleobj);
	if (self->release)
		self->release(self->handle);
	if (moduleobj->state_added) {
		/* This breaking of from the modal, since parent is added by
		 * parent and its upto the parent to del this object from path
		 * but since parent is not keeping track of cm_module
		 * its not possible to do del from parent at this time
		 * @todo: add cm_set modules under parent so that parent is responsible
		 * for the final release.
		 * Challange, current modal is to tie the module lifetime with its
		 * children and not the parent. Hence may be treated as a special
		 * case.
		 * */
		cm_object_del(moduleobj);
	}
	free(self->module_entry_symbol);
	free(self);
}

struct cm_module * cm_module_create(void *module_handle,
				    const char *module_entry_symbol)
{
	assert(module_entry_symbol && module_entry_symbol[0]);
	struct cm_module *self = (struct cm_module *)calloc(1, sizeof(*self));
	if (!self) {
		cm_error("Unable to allocate enough memory %d", errno);
		abort();
	}
	cm_object_init(&self->cmobj);
	self->cmobj.release = &cm_module_release;
	self->handle = module_handle;
	self->module_entry_symbol = strndup(module_entry_symbol,
					    strlen(module_entry_symbol));
	if (!self->module_entry_symbol) {
		cm_error("Unable to allocate enough memory %d", errno);
		abort();
	}
	return self;
}
