#ifndef _CM_MANAGER_PRIVATE_H_
#define _CM_MANAGER_PRIVATE_H_

#include "cm_list.h"
#include "cm_manager.h"
#include "cm_manager_iface.h"

struct cm_manager {
	struct cm_ref refcount;
	// list of dynamically loaded cm_manager interfaces
	struct cm_list_head ifaces;
	unsigned int num_ifaces;
//	cm_list *manager_face_handles;
//	cm_list *manager_ifaces;
};

#endif /* _CM_MANAGER_PRIVATE_H_ */
