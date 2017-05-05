#ifndef _CM_LOG_H_
#define _CM_LOG_H_

#include <syslog.h>

#if defined(CM_DEBUG)

#define cm_syslog(level, x, ...)	do {\
	switch(level) {\
	case LOG_ERR:\
		     syslog(LOG_ERR, "<error> %s#%s:%d " x, __FILE__,\
			    __func__, __LINE__,##__VA_ARGS__);\
		break;\
	case LOG_INFO:\
		      syslog(LOG_INFO, "<info> %s#%s:%d " x, __FILE__,\
			     __func__, __LINE__, ##__VA_ARGS__);\
		break;\
	case LOG_WARNING:\
			 syslog(LOG_WARNING, "<warn> %s#%s:%d " x, __FILE__,\
				__func__, __LINE__, ##__VA_ARGS__);\
		break;\
	case LOG_DEBUG:\
		       syslog(LOG_DEBUG, "<debug> %s#%s:%d " x, __FILE__,\
			      __func__, __LINE__, ##__VA_ARGS__);\
		break;\
	default:\
		syslog(LOG_WARNING, "%s#%s:%d " x, __FILE__,\
		       __func__, __LINE__, ##__VA_ARGS__);\
		break;\
	}\
} while(0)

#define cm_debug(x...)		cm_syslog(LOG_DEBUG, x)

#else

#define cm_syslog(level, x...)	do{\
	syslog(level, x);\
}while(0)

#define cm_debug(x...)		do {\
}while(0)

#endif

#define cm_error(x...)		cm_syslog(LOG_ERR, x)
#define cm_info(x...)		cm_syslog(LOG_INFO, x)
#define cm_warn(x...)		cm_syslog(LOG_WARNING, x)

#endif /* _CM_LOG_H_ */
