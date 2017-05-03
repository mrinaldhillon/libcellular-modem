#ifndef _CMM_MANAGER_PRIV_
#define _CMM_MANAGER_PRIV_

struct cmm_manager_priv {
	struct cm_set *cmm;
	cm_atomic_t num_cmm;
};

#endif /* _CMM_MANAGER_PRIV_ */
