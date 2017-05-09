#ifndef _CM_SIGNAL_H_
#define _CM_SIGNAL_H_

#include "cm_signal_type.h"

struct cm_signal;

struct cm_signal * cm_signal_ref(struct cm_signal *self);

void cm_signal_unref(struct cm_signal *self);

cm_signal_type_t cm_signal_get_type(struct cm_signal *self);

double cm_signal_get_rssi(struct cm_signal *self);

double cm_signal_get_ecio(struct cm_signal *self);

double cm_signal_get_sinr(struct cm_signal *self);

double cm_signal_get_io(struct cm_signal *self);

double cm_signal_get_rsrp(struct cm_signal *self);

double cm_signal_get_rsrq(struct cm_signal *self);

double cm_signal_get_snr(struct cm_signal *self);

#endif /* _CM_SIGNAL_H_ */
