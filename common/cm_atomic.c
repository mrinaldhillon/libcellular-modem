#include <stdlib.h>
#include <assert.h>

#if defined(__KERNEL_ATOMIC)

#else /* !__KERNEL_ATOMIC */

#include "cm_atomic.h"

#if defined(__GCC_ATOMIC) || defined(__GCC_ATOMIC_LEGACY)

int cm_atomic_read(const cm_atomic_t *v)
{
	assert(NULL != v);
	__sync_synchronize();
	v->counter;
}

void cm_atomic_set(cm_atomic_t *v, int i)
{
	assert(NULL != v);
	v ->counter= i;
	__sync_synchronize();
}

void cm_atomic_inc(cm_atomic_t *v)
{
	assert(NULL != v);
	(void)__sync_fetch_and_add(&v->counter, 1);
}

void cm_atomic_dec(cm_atomic_t *v)
{
	assert(NULL != v);
	(void)__sync_fetch_and_sub(&v->counter, 1);
}

int cm_atomic_dec_and_test(cm_atomic_t *v)
{
	assert(NULL != v);
	return 1 == (int)__sync_fetch_and_sub(&v->counter, 1) ? 1 : 0;
}
#else /* !__GCC_ATOMIC && !__GCC_ATOMIC_LEGACY */

/*
 * This is the default way. It is not very fast since mutex is global,
 * all ref, unref opterations in this namespace will take this lock.
 * TODO: lock variable can be part of the cm_atomic_t, this way each atomic
 * variable will have associate mutex, which shpuld speed up reference counting
 * in muti threaded environment, where references are being taken in unrelated
 * objects. Challenge is to achieve this wihout providing init and deinit fn.
 * for cm_atomic_t, since that won't conform with statndard atomic api.
 */

#include <pthread.h>

static pthread_mutex_t cm_atomic_lock = PTHREAD_MUTEX_INITIALIZER;

int cm_atomic_read(const cm_atomic_t *v)
{
	int value = 0;
	assert(NULL != v);
	pthread_mutex_lock(&cm_atomic_lock);
	value = v->counter;
	pthread_mutex_unlock(&cm_atomic_lock);
	return value;
}

void cm_atomic_set(cm_atomic_t *v, int i)
{
	assert(NULL != v);
	pthread_mutex_lock(&cm_atomic_lock);
	v->counter = i;
	pthread_mutex_unlock(&cm_atomic_lock);
}

void cm_atomic_inc(cm_atomic_t *v)
{
	assert(NULL != v);
	pthread_mutex_lock(&cm_atomic_lock);
	v->counter += 1;
	pthread_mutex_unlock(&cm_atomic_lock);
}

void cm_atomic_dec(cm_atomic_t *v)
{
	assert(NULL != v);
	pthread_mutex_lock(&cm_atomic_lock);
	v->counter -= 1;
	pthread_mutex_unlock(&cm_atomic_lock);
}

int cm_atomic_dec_and_test(cm_atomic_t *v)
{
	int is_zero;

	assert(NULL != v);
	pthread_mutex_lock(&cm_atomic_lock);
	v->counter -= 1;
	is_zero = 0 == v->counter ? 1 : 0;
	pthread_mutex_unlock(&cm_atomic_lock);

	return is_zero;
}

#endif /* __GCC_ATOMIC || __GCC_ATOMIC_LEGACY */
#endif /* __KERNEL_ATOMIC */

