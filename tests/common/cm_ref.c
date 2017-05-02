#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cm_container_of.h"
#include "cm_ref.h"
#include "cm_log.h"

struct testref {
	struct cm_ref refcount;
	void (*notify)(struct testref *self, void *userdata);
	void *notify_with_userdata;
	int i;
};

static void testref_release(struct cm_ref *ref)
{
	assert(NULL != ref);
	struct testref *self =
		cm_container_of(ref, struct testref, refcount);
	assert(NULL != self);
	printf("calling release notification\n");

	if (self->notify)
		self->notify(self, self->notify_with_userdata);

	printf("destroying the object\n");
	free(self);
}

static struct testref * testref_new(void)
{
	struct testref *obj = (struct testref *)calloc(1, sizeof(*obj));
	if (NULL == obj)
		abort();
	obj->i = 1;
	cm_ref_init(&obj->refcount);
	return obj;
}



static struct testref * testref_ref(struct testref *self)
{
	assert(NULL != self);
	cm_ref_get(&self->refcount);
	return self;
}

static void testref_unref(struct testref *self)
{
	assert(NULL != self);
	cm_ref_put(&self->refcount, &testref_release);
}

static void testref_notify_release(struct testref *self,
				   void (*notify)(struct testref *self,
						  void *userdata),
				   void *userdata)
{
	assert(NULL != self && NULL != notify);
	if (self->notify) {
		cm_warn("release notification can't be overridden\n");
		return;
	}
	self->notify = notify;
	self->notify_with_userdata = userdata;
}

static void main_to_be_notified_on_testref_release(struct testref *self,
						   void *userdata)
{
	printf("main has been notified, get out quickly\n");
}

int main()
{
	struct testref *obj = testref_new();
	if (NULL == obj)
		abort();
	testref_notify_release(obj,
			       &main_to_be_notified_on_testref_release,
			       NULL);
	testref_ref(obj);
	testref_unref(obj);
	testref_unref(obj);
	return 0;
}
