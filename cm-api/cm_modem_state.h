#ifndef _CM_MODEM_STATE_
#define _CM_MODEM_STATE_

#include <assert.h>

typedef enum {
	CM_MODEM_STATE_FAILED = -1,
	CM_MODEM_STATE_UNKNOWN = 0,
	CM_MODEM_STATE_INITIALIZING = 1,
	CM_MODEM_STATE_LOCKED = 2,
	CM_MODEM_STATE_DISABLED	= 3,
	CM_MODEM_STATE_DISABLING = 4,
	CM_MODEM_STATE_ENABLING = 5,
	CM_MODEM_STATE_ENABLED = 6,
	CM_MODEM_STATE_SEARCHING = 7,
	CM_MODEM_STATE_REGISTERED = 8,
	CM_MODEM_STATE_DISCONNECTING = 9,
	CM_MODEM_STATE_CONNECTING = 10,
	CM_MODEM_STATE_CONNECTED = 11
} cm_modem_state_t;

typedef enum {
	CM_MODEM_POWER_STATE_UNKNOWN = 0,
	CM_MODEM_POWER_STATE_OFF = 1,
	CM_MODEM_POWER_STATE_LOW = 2,
	CM_MODEM_POWER_STATE_ON
} cm_modem_power_state_t;

static inline const char * cm_modem_state_to_type(cm_modem_state_t state)
{
	assert(state <= CM_MODEM_STATE_CONNECTED &&
	       state >= CM_MODEM_STATE_FAILED);

	switch(state) {
		case CM_MODEM_STATE_FAILED:
			return "FAILED";
		case CM_MODEM_STATE_UNKNOWN:
			return "UNKNOWN";
		case CM_MODEM_STATE_INITIALIZING:
			return "INITIALIZING";
		case CM_MODEM_STATE_LOCKED:
			return "LOCKED";
		case CM_MODEM_STATE_DISABLED:
			return "DISABLED";
		case CM_MODEM_STATE_DISABLING:
			return "DISABLING";
		case CM_MODEM_STATE_ENABLING:
			return "ENABLING";
		case CM_MODEM_STATE_ENABLED:
			return "ENABLED";
		case CM_MODEM_STATE_SEARCHING:
			return "SEARCHING";
		case CM_MODEM_STATE_REGISTERED:
			return "REGISTERED";
		case CM_MODEM_STATE_DISCONNECTING:
			return "DISCONNECTING";
		case CM_MODEM_STATE_CONNECTING:
			return "CONNECTING";
		case CM_MODEM_STATE_CONNECTED:
			return "CONNECTED";
		default:
			return "INVALID STATE";
	}
}

#endif /* _CM_MODEM_STATE_ */
