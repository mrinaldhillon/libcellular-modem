#ifndef _CM_ATOMIC_H_
#define _CM_ATOMIC_H_

/* cm_atomic provides atomic operations on integer
 * it is a wrapper on other know iplementation of atomic operations
 * Based on target platform the supported implementation
 * should be compiled using compile time flags.
 * @todo: wrapper on gcc __atomic and c11 stdatomic
 **/

typedef struct cm_atomic {
	volatile int counter;
} cm_atomic_t;

#define CM_ATOMIC_INIT(i)  { (i) }

int cm_atomic_read(const cm_atomic_t *v);
void cm_atomic_set(cm_atomic_t *v, int i);
void cm_atomic_inc(cm_atomic_t *v);
int cm_atomic_inc_and_read(cm_atomic_t *v);
int cm_atomic_inc_not_zero(cm_atomic_t *v);
void cm_atomic_dec(cm_atomic_t *v);
int cm_atomic_dec_and_read(cm_atomic_t *v);
int cm_atomic_dec_and_test(cm_atomic_t *v);

#endif /* _CM_ATOMIC_H_ */

