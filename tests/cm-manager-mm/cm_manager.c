#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "libcm-manager.h"

#define APN	"12655.mcs"

static void cm_manager_test_print_modem_info(struct cm_modem *modem)
{
	assert(modem);
	cm_err_t err = CM_ERR_NONE;


	printf("Hardware:------------------------\n");
	printf("Manufacturer:\t%s\n", cm_modem_get_manufacturer(modem));
	printf("Model:\t%s\n", cm_modem_get_model(modem));
	printf("Equipment Id:\t%s\n", cm_modem_get_equipment_id(modem));
	printf("Status:--------------------------\n");
	printf("State:\t%s\n",
	       cm_modem_state_to_type(cm_modem_get_state(modem, &err)));
	printf("Signal Quality:\t%u\n", cm_modem_get_signal_quality(modem));
}

static void print_bearer_info(struct cm_bearer *bearer)
{
	cm_err_t err = CM_ERR_NONE;
	struct cm_bearer_ip_config *ip_conf=
		cm_bearer_get_ip_config(bearer, &err);
	if (CM_ERR_NONE != err) {
		printf("Error in getting ip config %d\n", err);
		return;
	}

	printf("Data Packet Bearer Info-----------\n");
	printf("IP Config: ----------------\n");
	printf("Method:\t%s\n",
	       cm_ip_method_to_string(cm_bearer_ip_config_get_method(ip_conf)));
	printf("Address:\t%s\n", cm_bearer_ip_config_get_address(ip_conf));
	printf("Prefix:\t%u\n", cm_bearer_ip_config_get_prefix(ip_conf));
	printf("Gateway:\t%s\n", cm_bearer_ip_config_get_gateway(ip_conf));
	printf("DNS:\t%s\n", cm_bearer_ip_config_get_dns(ip_conf));
	printf("MTU:\t%u\n", cm_bearer_ip_config_get_mtu(ip_conf));
	printf("Interface\t%s\n", cm_bearer_get_interface(bearer, &err));

	cm_bearer_ip_config_unref(ip_conf);
}

static void cm_manager_list_modems_for_each_ready(struct cm_manager *manager,
						  struct cm_modem *modem,
						  void *userdata)
{
	assert(manager && modem);
	cm_err_t err = CM_ERR_NONE;
	struct cm_bearer_properties * properties = NULL;
	struct cm_bearer *bearer = NULL;
	char *bearer_path = NULL;

	char *path = cm_modem_get_path(modem);
	printf("Modem Path: %s\n", path);


	cm_manager_test_print_modem_info(modem);
	printf("Creating Data Packet Bearer -------\n");

	properties = cm_bearer_properties_new(APN, NULL, NULL,
					      CM_IP_TYPE_IPV4, 1, NULL);
	bearer = cm_modem_create_bearer(modem, properties, &err);
	if (CM_ERR_NONE != err) {
		printf("Error in creating data packet bearer %d", err);
		goto out_unref_bearer_properties;
	}

	bearer_path = cm_bearer_get_path(bearer);
	printf("Bearer Path: %s\n", bearer_path);

	printf("Connecting Modem %s -------------\n", path);
	cm_bearer_connect(bearer, &err);
	if (CM_ERR_NONE != err) {
		printf("Error in connecting modem %d\n", err);
		goto out_unref_bearer;
	}
	printf("Connected\n");

//	print_bearer_info(bearer);

	printf("Press a key to continue\n");
	getchar();

	printf("Dis-connecting Modem %s -------------\n", path);
	cm_bearer_disconnect(bearer, &err);
	if (CM_ERR_NONE != err) {
		printf("Error in connecting modem %d\n", err);
		goto out_unref_bearer;
	}
	printf("Dis-connected-------------\n");

	printf("Deleting Bearer %s--------------\n", bearer_path);
	cm_modem_delete_bearer(modem, bearer_path, &err);
	if (CM_ERR_NONE != err) {
		printf("Error in deleting bearer %d\n", err);
	}
	printf("Press a key to continue\n");
	getchar();

out_unref_bearer:
	cm_bearer_unref(bearer);
	free(bearer_path);
out_unref_bearer_properties:
	cm_bearer_properties_unref(properties);
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
		goto out_unref;
	}

/*	printf("Connect Modems ----------\n");

	printf("Monitoring -----------\n");

	printf("Disconnect Modem -------------\n");
*/
	printf("Winding down\n");

out_unref:
	cm_manager_unref(manager);
	free(path);
	return err;
}
