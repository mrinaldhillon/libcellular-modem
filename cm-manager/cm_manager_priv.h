#ifndef _CM_MANAGER_PRIV_
#define _CM_MANAGER_PRIV_

struct cm_manager_priv {
	struct cm_set *modems;
	cm_atomic_t num_modems;
};

#endif /* _CMM_MANAGER_PRIV_ */
