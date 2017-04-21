#include <assert.h>
#include <stdlib.h>
#include "cm_ref.h"
#include "cm_atomic.h"

void cm_ref_init(struct cm_ref *self)
{
	assert(NULL != self);
	cm_atomic_set(&self->refcount, 1);
}

void cm_ref_get(struct cm_ref *self)
{
	assert(NULL != self);
	cm_atomic_inc(&self->refcount);
}

int cm_ref_peek(struct cm_ref *self)
{
	assert(NULL != self);
	cm_atomic_read(&self->refcount);
}

void cm_ref_put(struct cm_ref *self)
{
	assert(NULL != self);
	cm_atomic_dec(&self->refcount);
}

int cm_ref_put_is_final(struct cm_ref *self)
{
	assert(NULL != self);
	return cm_atomic_dec_and_test(&self->refcount);
}
