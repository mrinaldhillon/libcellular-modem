#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "libcmm-manager.h"

void cmm_manager_new_ready(struct cmm_manager *manager,
			  void *userdata,
			  cm_err_t err)
{
	char *path = NULL;
	printf("mydata: %s\n", (char *)userdata);
	if (CM_ERR_NONE != err){
		printf("Error in creating CMMManager %d", err);
		return;
	}

	path = cmm_manager_get_path(manager);
	printf("Succesfully created cmm manager %s\n", path);
	cmm_manager_start(manager, &err);
	cmm_manager_stop(manager, &err);
	printf("Winding down\n");
	free(path);
}

int main()
{
	cmm_manager_new_async(&cmm_manager_new_ready, "MYDATA");
	pthread_exit(NULL);
}
