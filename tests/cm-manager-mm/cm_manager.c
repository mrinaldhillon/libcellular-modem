#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "libcm-manager.h"

static void cm_manager_list_modems_for_each_ready(struct cm_manager *manager,
						  struct cm_modem *modem,
						  void *userdata)
{
	assert(manager && modem);
	char *path = cm_modem_get_path(modem);
	printf("Modem Path: %s\n", path);
	free(path);
}

int main()
{
	char *path = NULL;
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager *manager = cm_manager_new(&err);
	if (CM_ERR_NONE != err) {
		printf("Error in createing manager %d", err);
		return err;
	}

	path = cm_manager_get_path(manager);
	printf("Succesfully created cm manager %s\n", path);
	cm_manager_list_modems(manager,
			       &cm_manager_list_modems_for_each_ready,
			       NULL, &err);
	if (CM_ERR_NONE != err) {
		printf("Error in listing modems %d\n", err);
	}
	printf("Winding down\n");
	cm_manager_unref(manager);
	free(path);
	return 0;
}
