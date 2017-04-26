#ifndef _CM_SIGNAL_H_
#define _CM_SIGNAL_H_

#include "cm_signal_type.h"

struct cm_signal;

struct cm_signal * cm_signal_ref(struct cm_signal *self);
void cm_signal_unref(struct cm_signal *self);

#endif /* _CM_SIGNAL_H_ */
