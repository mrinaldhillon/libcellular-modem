#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cm_err.h>
#include <cm_thread.h>


static void * hello_world(void *data)
{
	printf("hello world\n");
	return NULL;
}

int main() {
	cm_err_t err = CM_ERR_NONE;
	cm_thread_t thread_id;

	cm_thread_create(&thread_id, &hello_world,
			 NULL, CM_THREAD_CREATE_DETACHED,
			 &err);
	if (CM_ERR_NONE != err) {
		printf("Error in creating thread %d\n", err);
	}
	pthread_exit(NULL); // required to allow hello world to execute, otherwise use join
}
