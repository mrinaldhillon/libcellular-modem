#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "cm_err.h"
#include "cm_log.h"
#include "cm_thread.h"

/**
 * This is the wrapper function for pthread_attr_init and pthread_attr_setdetachstate
 * @param attr, variable of type pthread_attr_t
 * @param detachedstate, thread state [PTHREAD_CREATE_JOINABLE or PTHREAD_CREATE_DETACHABLE]
 * @return returns int, 0 on success
 */
static void cm_thread_set_thread_attr(cm_thread_attr_t *attr, int detachedstate,
				      cm_err_t *err)
{
	int ret = CM_ERR_NONE;
	assert(attr);

	ret = pthread_attr_init(attr);
	if (ENOMEM == errno) {
		cm_error("Unable to allocate enough memory %d", errno);
		abort();
	} else if (0 != ret) {
		cm_warn("Error in initializing thread attributes : %d\n", ret);
		*err |= CM_ERR_THREAD_INIT_ATTR;
		return;
	}

	ret = pthread_attr_setdetachstate(attr, detachedstate);
	if (0 != ret) {
		cm_warn("Error in setting detach state for thread attributes: \
			%d\n", ret);
		*err |= CM_ERR_THREAD_SET_DETACHED_STATE;
		goto out_attrdestroy;
	}
	return;

out_attrdestroy:
	pthread_attr_destroy(attr);
}

static void cm_thread_destroy_thread_attr(cm_thread_attr_t *attr)
{
	assert(attr);
	pthread_attr_destroy(attr);
}

void cm_thread_create(cm_thread_t *thread,
		      cm_thread_start_routine_t start_routine,
		      cm_thread_arg_t arg, int detachedstate,
		      cm_err_t *err)
{
	cm_thread_attr_t attr;
	int ret = CM_ERR_NONE;

	assert(thread);
	cm_thread_set_thread_attr(&attr, detachedstate, err);
	if (CM_ERR_NONE != *err) {
		cm_warn("Error in setting thread attributes : %d\n", *err);
		return;
	}

	ret = pthread_create(thread, &attr, start_routine, arg);
	if (0 != ret) {
		cm_warn("Error in creating thread : %d\n", ret);
		*err |= CM_ERR_THREAD_CREATE;
	}

	cm_thread_destroy_thread_attr(&attr);
}
