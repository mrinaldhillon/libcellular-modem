#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "libcm-manager.h"

void cm_manager_new_ready(struct cm_manager *manager,
			  void *userdata,
			  cm_err_t err)
{
	char *path = NULL;
	printf("mydata: %s\n", (char *)userdata);
	if (CM_ERR_NONE != err){
		printf("Error in creating CMManager %d", err);
		return;
	}

	path = cm_manager_get_path(manager);
	printf("Succesfully created cm manager %s\n", path);
	cm_manager_start(manager, &err);
	cm_manager_stop(manager, &err);
	printf("Winding down\n");
	free(path);
}

int main()
{
	cm_manager_new_async(&cm_manager_new_ready, "MYDATA");
	pthread_exit(NULL);
}
