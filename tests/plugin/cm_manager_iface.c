#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "cm_plugin_handler.h"
#include "cm_manager_iface.h"

#define PLUGIN_DIR_PATH		"./install/usr/lib/cm-manager/plugins"

static void test_for_each_loaded(struct cm_manager_iface *iface,
			    void *userdata)
{
	assert(NULL != iface);
	printf("loaded %s\n", (char *)userdata);
}

static void test_load_plugin_done(void *userdata,
			     cm_err_t err)
{
	printf("done loading plugins with err: %d\n", err);
}

int main()
{
	char *dirpath = PLUGIN_DIR_PATH;
	cm_plugin_handler_load_from_dir(dirpath, 0,
					&test_for_each_loaded,
					&test_load_plugin_done,
					"1st iface");
	cm_plugin_handler_load_from_dir(dirpath, 0,
					&test_for_each_loaded,
					&test_load_plugin_done,
					"2nd iface");
	exit(0);
}
