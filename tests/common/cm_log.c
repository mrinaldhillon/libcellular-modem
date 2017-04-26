#include "cm_log.h"

int main()
{
	int i = 3;
	const char *message = "Blah";
	cm_error("Logging test %s, %d", message, i);
	cm_warn("Logging test");
	cm_info("Logging test %s, %d", message, i);
	cm_debug("Logging test %s, %d", message, i);
	return 0;
}
