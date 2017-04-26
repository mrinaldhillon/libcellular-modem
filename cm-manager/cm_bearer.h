#ifndef _CM_BEARER_H_
#define _CM_BEARER_H_

#include "cm_bearer_properties.h"
#include "cm_bearer_stats.h"
#include "cm_bearer_ip_config.h"

struct cm_bearer;

struct cm_bearer * cm_bearer_ref(struct cm_bearer *self);

void cm_bearer_unref(struct cm_bearer *self);

#endif /* _CM_BEARER_H_ */
