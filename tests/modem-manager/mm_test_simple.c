#include <stdio.h>
#include <stdlib.h>
#include <libmm-glib.h>

MMManager *manager;
GDBusConnection *dbus_conn;
GList *modems;
MMSimpleConnectProperties *simple_properties;
GError *gerr;

void handle_bearer(MMBearer *bearer)
{
	printf("Interface %s\n", mm_bearer_get_interface(bearer));

}

void handle_modem(MMObject *mmobj)
{
	MMModem *modem = mm_object_get_modem(mmobj);
	MMModemSimple *simple_modem = mm_object_get_modem_simple(mmobj);

	g_dbus_proxy_set_default_timeout(G_DBUS_PROXY(simple_modem), 30 * 1000);
	printf("Modem Path: %s\n", mm_modem_simple_get_path(simple_modem));


	MMBearer * bearer = mm_modem_simple_connect_sync(simple_modem,
					simple_properties, NULL , &gerr);
	if (gerr) {
		printf("Error in connecting %s", gerr->message);
		goto out_unref_modem;
	}
	const char * bearer_path = mm_bearer_get_path(bearer);
	printf("Created bearer %s\n", bearer_path);
	handle_bearer(bearer);

	mm_modem_simple_disconnect_sync(simple_modem, bearer_path, NULL, &gerr);
	if (gerr) {
		printf("Error in disconnecting %s", gerr->message);
		goto out_unref_bearer;
	}

out_unref_bearer:
	mm_modem_delete_bearer_sync(modem, bearer_path, NULL, &gerr);
	g_object_unref(bearer);
out_unref_modem:
	g_object_unref(modem);
}

void handle_modems()
{
	GList *each = NULL;
	modems =
	g_dbus_object_manager_get_objects(G_DBUS_OBJECT_MANAGER(manager));
	for (each = modems; each; each = g_list_next(each)) {

		handle_modem(MM_OBJECT(each->data));
	}
	g_list_free_full(modems, (GDestroyNotify) g_object_unref);
}

void create_simple_properties()
{
	simple_properties = mm_simple_connect_properties_new();
	if (NULL == simple_properties) {
		printf("Error in creating properties");
		return;
	}

	mm_simple_connect_properties_set_apn(simple_properties, "12655.mcs");
	mm_simple_connect_properties_set_ip_type(simple_properties, MM_BEARER_IP_FAMILY_IPV4);
}
void create_manager()
{
	gchar * name_owner = NULL;
	dbus_conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &gerr);
	if (gerr) {
		printf("Error in getting dbus conn %s", gerr->message);
		goto out_gerr;
	}

	manager =
		mm_manager_new_sync(dbus_conn,
				      G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_DO_NOT_AUTO_START,
				      NULL, &gerr);
	if (gerr) {
		printf("Error in creating new modem manager %s", gerr->message);
		goto out_unref_dbus_conn;
	}
	name_owner = g_dbus_object_manager_client_get_name_owner (G_DBUS_OBJECT_MANAGER_CLIENT (manager));
	if (name_owner) {
		printf("Owner %s\n", name_owner);
	}
	g_dbus_proxy_set_default_timeout (mm_manager_peek_proxy(manager), 30 * 1000);
	create_simple_properties();
	if(gerr) {
		printf("Error in creating properties %s", gerr->message);
		goto out_unref_manager;
	}
	handle_modems();

	g_object_unref(simple_properties);
out_unref_manager:
	g_object_unref(manager);
out_unref_dbus_conn:
	g_object_unref(dbus_conn);
out_gerr:
	if(gerr)
	g_error_free(gerr);
}

int main()
{
	create_manager();
	return 0;
}
