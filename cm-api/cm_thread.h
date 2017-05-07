#ifndef _CM_THREAD_H_
#define _CM_THREAD_H_

#include <pthread.h>
#include "cm_err.h"

#define cm_thread_t				pthread_t
#define cm_thread_attr_t			pthread_attr_t

typedef void*(*cm_thread_start_routine_t)(void*);
#define cm_thread_arg_t		void*

void cm_thread_create(cm_thread_t *thread,
		      cm_thread_start_routine_t start_routine,
		      cm_thread_arg_t arg, int detachedstate,
		      cm_err_t *err);

#define CM_THREAD_CREATE_DETACHED	PTHREAD_CREATE_DETACHED
#define CM_THREAD_CREATE_JOINABLE	PTHREAD_CREATE_JOINABLE
#define cm_thread_exit			pthread_exit
#define cm_thread_detach		pthread_detach
#define cm_thread_mutex_t		pthread_mutex_t
#define cm_thread_mutexattr_t		pthread_mutexattr_t
#define cm_mutex_init(mutex,attr)	pthread_mutex_init(&(mutex),attr)
#define cm_mutex_destroy(mutex)		pthread_mutex_destroy(&mutex)
#define cm_thread_id			pthread_self()

#define cm_thread_mutex_lock(x)		pthread_mutex_lock(&(x))
#define cm_thread_mutex_trylock(x)	pthread_mutex_trylock(&(x))
#define cm_thread_mutex_unlock(x)	pthread_mutex_unlock(&(x))

#endif /* _CM_THREAD_H_ */
