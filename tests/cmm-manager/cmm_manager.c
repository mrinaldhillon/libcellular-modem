#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "libcmm-manager.h"

int main()
{
	cm_err_t err = CM_ERR_NONE;
	char *path = NULL;
	struct cmm_manager *manager = cmm_manager_new(&err);
	if (CM_ERR_NONE != err)
		return -1;
	path = cmm_manager_get_path(manager);
	cmm_manager_start(manager, &err);
	cmm_manager_stop(manager, &err);
	printf("Succesfully created CMMManager: %s\n Winding down\n", path);
	cmm_manager_unref(manager);
	free(path);
	pthread_exit(0);
}
