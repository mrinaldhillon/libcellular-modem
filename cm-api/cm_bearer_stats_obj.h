#ifndef _CM_BEARER_STATS_OBJ_H_
#define _CM_BEARER_STATS_OBJ_H_

#include "cm_err.h"
#include <sys/types.h>

struct cm_bearer_stats_priv;

struct cm_bearer_stats {
	struct cm_object cmobj;

	struct cm_bearer_stats_priv *priv;

	struct cm_bearer_stats * (*get)(struct cm_bearer_stats *self);

	void (*put)(struct cm_bearer_stats *self);

	unsigned int (*get_duration)(struct cm_bearer_stats *self);

	int64_t (*get_rx_bytes)(struct cm_bearer_stats *self);

	int64_t (*get_tx_bytes)(struct cm_bearer_stats *self);
};

struct cm_bearer_stats * cm_bearer_obj_stats_new(cm_err_t *err);


#endif /* _CM_BEARER_STATS_OBJ_H_ */
