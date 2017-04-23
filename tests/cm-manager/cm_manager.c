#include <stdio.h>
#include "cm_err.h"
#include "cm_manager.h"

int main()
{
	cm_err_t err = CM_ERR_NONE;
	struct cm_manager *manager = cm_manager_new(&err);
	if (CM_ERR_NONE != err)
		return -1;

	printf("Succesfully created cm manager\n Winding down\n");
	cm_manager_unref(manager);
	return 0;
}