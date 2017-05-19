#ifndef _CM_BEARER_STATS_PRIV_H_
#define _CM_BEARER_STATS_PRIV_H_

#include <sys/types.h>

struct cm_bearer_priv {
	int64_t rx_bytes;
	int64_t tx_bytes;
	// other priv data here
};

#endif /* _CM_BEARER_STATS_PRIV_H_ */
