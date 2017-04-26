#ifndef _CM_BEARER_PROPERTIES_H_
#define _CM_BEARER_PROPERTIES_H_

struct cm_bearer_properties;

struct cm_bearer_properties *
cm_bearer_properties_ref(struct cm_bearer_properties *self);

void cm_bearer_properties_unref(struct cm_bearer_properties *self);

#endif /* _CM_BEARER_PROPERTIES_H_ */
