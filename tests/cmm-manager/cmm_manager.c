#include <stdio.h>
#include "cm_err.h"
#include "cmm_manager.h"

int main()
{
	cm_err_t err = CM_ERR_NONE;
	struct cmm_manager *manager = cmm_manager_new(&err);
	if (CM_ERR_NONE != err)
		return -1;
	cmm_manager_start(manager, &err);
	cmm_manager_stop(manager, &err);
	printf("Succesfully created cm manager\n Winding down\n");
	cmm_manager_destroy(manager);
	return 0;
}
