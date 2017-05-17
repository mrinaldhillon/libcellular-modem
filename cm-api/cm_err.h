#ifndef _CM_ERR_H_
#define _CM_ERR_H_

typedef enum {
	CM_ERR_NONE = 0,
	CM_ERR_EINVAL,
	CM_ERR_GENERAL,
	CM_ERR_THREAD_INIT_ATTR,
	CM_ERR_THREAD_SET_DETACHED_STATE,
	CM_ERR_THREAD_CREATE,
	CM_ERR_MODULE_LOADER_DLOPEN,
	CM_ERR_MODULE_LOADER_DLSYM,
	CM_ERR_MODULE_LOADER_DLCLOSE,
	CM_ERR_MODULE_LOADER_OPENDIR,
	CM_ERR_MODULE_LOADER_ELIBACC,
	CM_ERR_MMANAGER_NEW,
	CM_ERR_MANAGER_NEW,
	CM_ERR_MANAGER_LIST_MODEMS_ASYNC,
	CM_ERR_MANAGER_MM_GET_BUS,
	CM_ERR_MANAGER_MM_CREATE_MODEM_MANAGER,
	CM_ERR_MANAGER_MM_INIT_MODEM_MANAGER,
	CM_ERR_MODEM_MM_CREATE_BEARER,
	CM_ERR_MODEM_MM_DELETE_BEARER,
	CM_ERR_MODEM_MM_BEARER_NOT_FOUND,
	CM_ERR_BEARER_MM_CONNECT,
	CM_ERR_BEARER_MM_DISCONNECT,
	CM_ERR_BEARER_MM_GET_IP_CONFIG,
	CM_ERR_MODEM_MM_ENABLE,
	CM_ERR_MODEM_MM_DISABLE
} cm_err_t;

#endif /* _CM_ERR_H_ */
