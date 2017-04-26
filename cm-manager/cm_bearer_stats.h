#ifndef _CM_BEARER_STATS_H_
#define _CM_BEARER_STATS_H_

struct cm_bearer_stats;

struct cm_bearer_stats * cm_bearer_stats_ref(struct cm_bearer_stats *self);

void cm_bearer_stats_unref(struct cm_bearer_stats *self);

#endif /* _CM_BEARER_STATS_H_ */
