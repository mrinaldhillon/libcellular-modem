//#include <semaphore.h>
#include "cm_object.h"
#include "cm_module.h"
#include "cm_log.h"
//@todo: revisit unloading logic
//static sem_t mutex;
#define MODULE_NAME			"EXAMPLE_MODULE"

__attribute__((constructor)) void cm_module_example_constructor(void)
{
//	sem_init(&mutex, 0, 1);
	cm_debug("Loading plugin: %s", MODULE_NAME);
}
__attribute__((destructor)) void cm_module_example_destructor(void)
{
//	sem_wait(&mutex);
	cm_debug("Unloading plugin: %s", MODULE_NAME);
//	sem_destroy(&mutex);
}

void cm_module_init(struct cm_module *owner)
{
	cm_debug("Example module initialized");
}

const char * get_module_entry_symbol(void)
{
	return "cm_manager_init";
}
