#ifndef _CM_ATOMIC_H_
#define _CM_ATOMIC_H_

#include "cm_types.h"

int cm_atomic_int_get(const volatile int *atomic);
void cm_atomic_int_set(volatile int *atomic, int value);

void cm_atomic_int_inc(volatile int *atomic);
void cm_atmomic_int_dec(volatile int *atomic);
cm_boolean cm_atomic_int_dec_is_zero(volatile int *atomic);
cm_boolean cm_atomic_int_is_next_dec_zero(const volatile int *atomic);

#endif /* _CM_ATOMIC_H_ */
