#include <stdio.h>
#include "cm_atomic.h"

int main()
{
	printf("Initializing to 0");
	cm_atomic_t v = CM_ATOMIC_INIT(0);
	printf("read value %d\n", cm_atomic_read(&v));

	printf("setting value 1\n");
	cm_atomic_set(&v, 1);
	printf("read value %d\n", cm_atomic_read(&v));

	printf("increment\n");
	cm_atomic_inc(&v);
	printf("read value %d\n", cm_atomic_read(&v));

	printf("decrement\n");
	cm_atomic_dec(&v);
	printf("read value %d\n", cm_atomic_read(&v));

	printf("decrement and test is_zero=%d\n", cm_atomic_dec_and_test(&v));

	return 0;
}
