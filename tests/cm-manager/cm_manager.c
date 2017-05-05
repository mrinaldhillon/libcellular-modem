#include <stdio.h>
#include "libcm-manager.h"

int main()
{
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager *manager = cm_manager_new(&err);
	if (CM_ERR_NONE != err)
		return -1;
	cm_manager_start(manager, &err);
	cm_manager_stop(manager, &err);
	printf("Succesfully created cm manager\n Winding down\n");
	cm_manager_destroy(manager);
	return 0;
}
