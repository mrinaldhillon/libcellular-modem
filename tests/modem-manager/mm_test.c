#include <stdio.h>
#include <stdlib.h>
#include <libmm-glib.h>

MMManager *manager;
GDBusConnection *dbus_conn;
GList *modems;
MMBearerProperties *properties;
GError *gerr;
GList *bearers;

void handle_bearer(MMBearer *bearer)
{
	printf("Bearer Path %s\n", mm_bearer_get_path(bearer));
	printf("Interface %s\n", mm_bearer_get_interface(bearer));

}

void list_bearers(MMModem *modem)
{
	GList *each = NULL;
	bearers = mm_modem_list_bearers_sync(modem, NULL, &gerr);
	if (gerr) {
		printf("Error in listing bearers %s", gerr->message);
		return;
	}

	for (each = bearers; each; each = g_list_next(each)) {
		handle_bearer(MM_BEARER(each->data));
	}
	g_list_free_full(bearers, (GDestroyNotify) g_object_unref);

}

void handle_modem(MMObject *mmobj)
{
	MMModem *modem = mm_object_get_modem(mmobj);
	printf("Modem Path: %s\n", mm_modem_get_path(modem));
	MMBearer *bearer = mm_modem_create_bearer_sync(modem, properties,
							   NULL, &gerr);
	if (gerr) {
		printf("Error in creating bearer %s", gerr->message);
		goto out_unref_modem;
	}

	const char * bearer_path = mm_bearer_get_path(bearer);
	printf("Created bearer %s\n", bearer_path);
	mm_bearer_connect_sync(bearer, NULL , &gerr);
	if (gerr) {
		printf("Error in connecting %s", gerr->message);
		goto out_unref_bearer;
	}
	//printf("Interface %s\n", mm_bearer_get_interface(bearer));
	list_bearers(modem);
	getchar();

	mm_bearer_disconnect_sync(bearer, NULL, &gerr);
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

void create_bearer_properties()
{
	properties = mm_bearer_properties_new();
	if (NULL == properties) {
		printf("Error in creating properties");
		return;
	}

	mm_bearer_properties_set_apn(properties, "12655.mcs");
	mm_bearer_properties_set_ip_type(properties, MM_BEARER_IP_FAMILY_IPV4);
}

void create_manager()
{
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
	create_bearer_properties();
	if(gerr) {
		printf("Error in creating properties %s", gerr->message);
		goto out_unref_manager;
	}
	handle_modems();

	g_object_unref(properties);
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
