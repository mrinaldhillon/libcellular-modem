#ifndef _CM_MANAGER_PRIVATE_H_
#define _CM_MANAGER_PRIVATE_H_

#include "cm_atomic.h"
#include "cm_object.h"
#include "cm_manager.h"
#include "cm_manager_iface.h"

struct cm_manager {
	struct cm_object cmobj;
	// list of dynamically loaded cm_manager interfaces
	struct cm_set *ifaces;
	cm_atomic_t num_ifaces;
};

#endif /* _CM_MANAGER_PRIVATE_H_ */
