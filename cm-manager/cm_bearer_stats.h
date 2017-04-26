#ifndef _CM_BEARER_STATS_H_
#define _CM_BEARER_STATS_H_

#include <sys/types.h>

struct cm_bearer_stats;

struct cm_bearer_stats * cm_bearer_stats_ref(struct cm_bearer_stats *self);

void cm_bearer_stats_unref(struct cm_bearer_stats *self);

unsigned int cm_bearer_stats_get_duration(struct cm_bearer_stats *self);

int64_t cm_bearer_stats_get_rx_bytes(struct cm_bearer_stats *self);

int64_t cm_bearer_stats_get_tx_bytes(struct cm_bearer_stats *self);

#endif /* _CM_BEARER_STATS_H_ */
