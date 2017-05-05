#ifndef _CMM_MANAGER_PRIV_H_
#define _CMM_MANAGER_PRIV_H_

#include "cm_atomic.h"
#include "cm_object.h"

struct cmm_manager_priv {
	struct cm_set *cmmset;
	cm_atomic_t num_cmm;
};

#endif /* _CMM_MANAGER_PRIV_H_ */
