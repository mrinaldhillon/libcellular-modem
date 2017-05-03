#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <link.h>
#include "cm_object.h"
#include "cm_module.h"
#include "cm_module_loader.h"

#if !defined(LIBMODULE_FOO_SODIR)
#define LIB_MODULE_FOO_SODIR	"./tests/common"
#endif

#define	LIB_MODULE_FOO_SONAME	"libcm-module-foo.so"

//#define CM_MODULE_FOO_SO_INIT_SYM	"cm_moule_foo_init"

typedef void (*cm_module_foo_init)(struct cm_module *module);

static void cm_module_loader_get_module_init_symbol(struct cm_module *module,
						    cm_err_t *err)
{
	char *error = NULL;
	cm_module_foo_init foo_init = NULL;
	foo_init = dlsym(module->handle, module->module_entry_symbol);
	if (NULL != (error = dlerror())) {
		printf("Symbol for module create \"%s\" not found %s",
			 module->module_entry_symbol, error);
		*err |= CM_ERR_MODULE_LOADER_DLSYM;
	}
	foo_init(module);
}

int main()
{
	cm_err_t err = CM_ERR_NONE;
	char *sopath = NULL;
	struct cm_module * module = NULL;

	sopath = cm_module_loader_build_filepath(LIB_MODULE_FOO_SODIR,
						 LIB_MODULE_FOO_SONAME,
						 &err);
	module = cm_module_loader_load_path(sopath, 1, &err);
	if (CM_ERR_NONE != err) {
		printf("Could not load module %d\n", err);
		goto out_free_path;
	}
	cm_object_set_name(&module->cmobj, "CMModule-1");
	printf("Module entry symbol %s\n", module->module_entry_symbol);
	cm_module_loader_get_module_init_symbol(module, &err);
	if (CM_ERR_NONE != err) {
		printf("Could not load entry symbol from library %d\n", err);
	}
	cm_object_put(&module->cmobj);
	printf("press key to exit\n");
	getchar();

out_free_path:
	free(sopath);
	exit(err);
}
