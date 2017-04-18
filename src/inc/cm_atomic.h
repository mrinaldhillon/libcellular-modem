#ifndef _CM_ATOMIC_H_
#define _CM_ATOMIC_H_

#if defined(__KERN_ATOMIC)

#include <asm/atomic.h>

#define CM_ATOMIC_INIT  ATOMIC_INIT
#define cm_atomic_t	atomic_t
#define cm_atomic_read	atomic_read
#define cm_atomic_set	atomic_set
#define cm_atomic_inc	atomic_inc
#define cm_atomic_dec	atomic_dec
#define cm_atomic_dec_and_test atomic_dec_and_test

#else /* !__KERN_ATOMIC */

//typedef struct cm_atomic cm_atomic_t;
typedef struct cm_atomic {
	volatile int value;
} cm_atomic_t;
#define CM_ATOMIC_INIT(i)  { (i) }

int cm_atomic_read(const cm_atomic_t *v);
void cm_atomic_set(cm_atomic_t *v, int i);
void cm_atomic_inc(cm_atomic_t *v);
void cm_atomic_dec(cm_atomic_t *v);
int cm_atomic_dec_and_test(cm_atomic_t *v);

#endif /* __KERN_ATOMIC */

#endif /* _CM_ATOMIC_H_ */

