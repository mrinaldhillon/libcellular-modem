#ifndef _CM_SIM_H_
#define _CM_SIM_H_

struct cm_sim;

struct cm_sim * cm_sim_ref(struct cm_sim *self);
void cm_sim_unref(struct cm_sim *self);

#endif /* _CM_SIM_H_ */
