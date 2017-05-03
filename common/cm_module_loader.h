#ifndef _CM_MODULE_LOADER_H_
#define _CM_MODULE_LOADER_H_

#include "cm_module.h"

typedef void (*for_each_module_loaded)(struct cm_module *module,
				void *userdata);
typedef void (*load_modules_done)(void *userdata,
				cm_err_t err);


struct cm_module * cm_module_loader_load_path(const char *path,
					      int load_in_new_namespace,
					      cm_err_t *err);

char * cm_module_loader_build_filepath(const char *dirpath,
				   const char *filename,
				   cm_err_t *err);

void cm_module_loader_load_from_dirpath(const char *dirpath,
					int load_in_new_namespace,
					for_each_module_loaded loaded,
					load_modules_done done,
					void *userdata,
					cm_err_t *err);

#endif /* _CM_MODULE_LOADER_H_ */
