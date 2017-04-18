#include "cm_atomic.h"
#include "cm_types.h"
#include <pthread.h>
#include <assert.h>

static pthread_mutex_t cm_atomic_lock = PTHREAD_MUTEX_INITIALIZER;

int cm_atomic_int_get(const volatile int *atomic)
{
	int value = 0;

	assert(NULL != atomic);
	pthread_mutex_lock(&cm_atomic_lock);
	value = *atomic;
	pthread_mutex_unlock(&cm_atomic_lock);

	return value;
}

void cm_atomic_int_set(volatile int *atomic, int value)
{
	assert(NULL != atomic);
	pthread_mutex_lock(&cm_atomic_lock);
	*atomic = value;
	pthread_mutex_unlock(&cm_atomic_lock);
}

void cm_atomic_int_inc(volatile int *atomic)
{
	assert(NULL != atomic);
	pthread_mutex_lock(&cm_atomic_lock);
	*atomic += 1;
	pthread_mutex_unlock(&cm_atomic_lock);
}

void cm_atomic_int_dec(volatile int *atomic)
{
	assert(NULL != atomic);
	pthread_mutex_lock(&cm_atomic_lock);
	*atomic -= 1;
	pthread_mutex_unlock(&cm_atomic_lock);
}

cm_boolean cm_atomic_int_dec_is_zero(volatile int *atomic)
{
	cm_boolean is_zero;

	assert(NULL != atomic);
	pthread_mutex_lock(&cm_atomic_lock);
	*atomic -= 1;
	is_zero = 0 == *atomic ? 1 : 0;
	pthread_mutex_unlock(&cm_atomic_lock);

	return is_zero;
}

cm_boolean cm_atomic_int_is_next_dec_zero(const volatile int *atomic)
{
	cm_boolean is_zero;

	assert(NULL != atomic);
	pthread_mutex_lock(&cm_atomic_lock);
	is_zero = 0 == (*atomic - 1) ? 1 : 0;
	pthread_mutex_unlock(&cm_atomic_lock);

	return is_zero;
}
