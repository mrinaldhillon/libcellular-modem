#include <stdlib.h>
#include <stdio.h>
#include "cm_object.h"
#include "cm_module.h"
#include "cm_module_loader.h"

#if !defined(LIBMODULE_EXAMPLE_SODIR)
#define LIB_MODULE_EXAMPLE_SODIR	"./tests/common"
#endif

#define	LIB_MODULE_EXAMPLE_SONAME	"./libcm-module-example.so"

int main()
{
	cm_err_t err = CM_ERR_NONE;
	char *sopath = NULL;
	struct cm_module * module = NULL;

	sopath = cm_module_loader_build_filepath(LIB_MODULE_EXAMPLE_SODIR,
						 LIB_MODULE_EXAMPLE_SONAME,
						 &err);
	module = cm_module_loader_load_path(sopath, 1, &err);
	if (CM_ERR_NONE != err) {
		printf("Could not load module %d\n", err);
		goto out_free_path;
	}
	printf("Module entry symbol %s\n", module->module_entry_symbol);
	cm_object_put(&module->cmobj);
	printf("press key to exit\n");
	getchar();

out_free_path:
	free(sopath);
	exit(err);
}
