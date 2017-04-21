#ifndef _CM_REF_H_
#define _CM_REF_H_

#include "cm_atomic.h"

struct cm_ref {
	cm_atomic_t refcount;
};

void cm_ref_init(struct cm_ref *self);
void cm_ref_get(struct cm_ref *self);
int cm_ref_peek(struct cm_ref *self);
void cm_ref_put(struct cm_ref *self);
int cm_ref_put_is_final(struct cm_ref *self);

#endif /* _CM_REF_H_ */
