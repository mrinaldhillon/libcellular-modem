#ifndef _CM_SIGNAL_OBJ_H_
#define _CM_SIGNAL_OBJ_H_

#include "cm_err.h"
#include "cm_object.h"
#include "cm_signal_type.h"

struct cm_signal_priv;

struct cm_signal {
	struct cm_object cmobj;

	struct cm_signal_priv *priv;

	const char * (*get_class_name)(void);

	char *(*get_path)(struct cm_signal *self);

	struct cm_signal * (*get)(struct cm_signal *self);

	void (*put)(struct cm_signal *self);

	cm_signal_type_t (*get_type)(struct cm_signal *self);

	double (*get_rssi)(struct cm_signal *self);

	double (*get_ecio)(struct cm_signal *self);

	double (*get_sinr)(struct cm_signal *self);

	double (*get_io)(struct cm_signal *self);

	double (*get_rsrp)(struct cm_signal *self);

	double (*get_rsrq)(struct cm_signal *self);

	double (*get_snr)(struct cm_signal *self);
};

struct cm_signal * cm_signal_new(cm_err_t *err);

#endif /* _CM_SIGNAL_OBJ_H_ */

