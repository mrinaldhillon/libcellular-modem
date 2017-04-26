#ifndef _CM_MESSAGING_H_
#define _CM_MESSAGING_H_

struct cm_messaging;

struct cm_messaging * cm_messaging_ref(struct cm_messaging *self);
void cm_messaging_unref(struct cm_messaging *self);

#endif /* _CM_MESSAGING_H_ */
