#ifndef _CM_BEARER_STATE_
#define _CM_BEARER_STATE_

/*
 * @tbd Not sure about suspended
 */
typedef enum {
	CM_BEARER_STATE_NONE = 0,
	CM_BEARER_STATE_CONNECTED,
	CM_BEARER_STATE_DISCONNECTED,
	CM_BEARER_STATE_SUSPENDED
} cm_bearer_state_t;

#endif /* _CM_BEARER_STATE_ */
