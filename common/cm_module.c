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
