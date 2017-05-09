#ifndef _CM_SIGNAL_PRIV_H_
#define _CM_SIGNAL_PRIV_H_

#include "cm_signal_type.h"

struct cm_signal_priv {
	double rssi;
	double ecio;
	double sinr;
	double io;
	double rsrp;
	double rsrq;
	double snr;
	cm_signal_type signal_type;
};

#endif /* _CM_SIGNAL_PRIV_H_ */

