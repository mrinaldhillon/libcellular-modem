//#include <semaphore.h>
#include "cm_object.h"
#include "cm_module.h"
#include "cm_log.h"
//@todo: revisit unloading logic
//static sem_t mutex;
#define MODULE_NAME			"FOO_MODULE"

__attribute__((constructor)) void cm_module_foo_constructor(void)
{
//	sem_init(&mutex, 0, 1);
	cm_debug("Loading plugin: %s", MODULE_NAME);
}
__attribute__((destructor)) void cm_module_foo_destructor(void)
{
//	sem_wait(&mutex);
	cm_debug("Unloading plugin: %s", MODULE_NAME);
//	sem_destroy(&mutex);
}

void cm_module_foo_init(struct cm_module *owner)
{
	cm_debug("Foo module initialized");
	if (owner)
		cm_debug("Owner Module:%s", cm_object_get_name(&owner->cmobj));
}

const char * cm_get_module_entry_symbol(void)
{
	return "cm_module_foo_init";
}
