#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "cm_plugin_handler.h"
#include "cm_manager_iface.h"

#if !defined(PLUGINS_DIR)
#define PLUGINS_DIR		"./install/usr/lib/cm-manager"
#endif

static void test_for_each_loaded(struct cm_manager_iface *iface,
			    void *userdata)
{
	assert(NULL != iface);
	printf("Loaded Iface name: %s\n", iface->get_name(iface));
}

static void test_load_plugin_done(void *userdata,
			     cm_err_t err)
{
	printf("done loading plugins with err: %d\n", err);
}

int main()
{
	cm_err_t err = CM_ERR_NONE;

	cm_plugin_handler_load_from_dir(PLUGINS_DIR, 1,
					&test_for_each_loaded,
					&test_load_plugin_done,
					"1st iface", &err);
	if (CM_ERR_NONE != err) {
		printf("Could load plugins %d", err);
		exit(1);
	}

	printf("press key to exit\n");
	getchar();
	exit(0);
}
