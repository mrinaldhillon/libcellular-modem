#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "libcm-manager.h"

static void cm_manager_test_print_modem_info(struct cm_modem *modem)
{
	assert(modem);
	cm_err_t err = CM_ERR_NONE;

	char *path = cm_modem_get_path(modem);
	printf("Path: %s\n", path);
	free(path);

	printf("Hardware:------------------------\n");
	printf("Manufacturer:\t%s\n", cm_modem_get_manufacturer(modem));
	printf("Model:\t%s\n", cm_modem_get_model(modem));
	printf("Equipment Id:\t%s\n", cm_modem_get_equipment_id(modem));
	printf("Status:--------------------------\n");
	printf("State:\t%s\n",
	       cm_modem_state_to_type(cm_modem_get_state(modem, &err)));
	printf("Signal Quality:\t%u\n", cm_modem_get_signal_quality(modem));
}

static void cm_manager_list_modems_for_each_ready(struct cm_manager *manager,
						  struct cm_modem *modem,
						  void *userdata)
{
	assert(manager && modem);
	cm_manager_test_print_modem_info(modem);
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
