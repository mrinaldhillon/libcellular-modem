#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "libcm-manager.h"

static void cm_manager_list_modems_for_each_ready(struct cm_manager *manager,
						  struct cm_modem *modem,
						  void *userdata)
{
	assert(manager && modem);
	char *path = cm_modem_get_path(modem);
	printf("CMModem path:%s\n", path);
	free(path);
}

static void cm_manager_list_modems_done_ready(struct cm_manager *manager,
						  void *userdata,
						  cm_err_t err)
{
	assert(manager);
	printf("Listing modems done with err %d\n", err);
}

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
	cm_manager_list_modems_async(manager,
				     &cm_manager_list_modems_for_each_ready,
				     &cm_manager_list_modems_done_ready,
				     "MYDATA");
	free(path);
}

int main()
{
	cm_manager_new_async(&cm_manager_new_ready, "MYDATA");
	printf("Press a key to exit\n");
	getchar();
	exit(0);
}
