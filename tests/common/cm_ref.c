#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ccan/container_of/container_of.h"
#include "cm_ref.h"

struct testref {
	struct cm_ref ref;
	int i;
};

static void release_ref(struct cm_ref *ref)
{
	assert(NULL != ref);
	struct testref *self =
		(struct testref *)container_of(ref, struct testref, ref);
	assert(NULL != self);
	printf("destroying the object");
	free(self);
}

static struct testref * testref_new(void)
{
	struct testref *obj = (struct testref *)calloc(sizeof(*obj), 1);
	if (NULL == obj)
		abort();
	obj->i = 1;
	cm_ref_init(&obj->ref);
	return obj;
}

static struct testref * testref_ref(struct testref *self)
{
	assert(NULL != self);
	cm_ref_get(&self->ref);
	return self;
}

static void testref_unref(struct testref *self)
{
	assert(NULL != self);
	cm_ref_put(&self->ref, &release_ref);
}

int main()
{
	struct testref *obj = testref_new();
	if (NULL == obj)
		abort();
	testref_ref(obj);
	testref_unref(obj);
	testref_unref(obj);
	return 0;
}
