#include <assert.h>
#include <stdlib.h>
#include "cm_atomic.h"

struct cm_ref {
	cm_atomic_t ref_count;
};

void cm_ref_init(struct cm_ref *self)
{
	assert(NULL != self);
	cm_atomic_set(&self->ref_count, 0);
}

void cm_ref_get(struct cm_ref *self)
{
	assert(NULL != self);
	cm_atomic_inc(&self->ref_count);
}

int cm_ref_peek(struct cm_ref *self)
{
	assert(NULL != self);
	cm_atomic_read(&self->ref_count);
}

void cm_ref_put(struct cm_ref *self)
{
	assert(NULL != self);
	cm_atomic_dec(&self->ref_count);
}

int cm_ref_put_is_final(struct cm_ref *self)
{
	assert(NULL != self);
	return cm_atomic_dec_and_test(&self->ref_count);
}


