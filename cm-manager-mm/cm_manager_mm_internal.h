#ifndef _CM_MANAGER_MM_INTERNAL_H_
#define _CM_MANAGER_MM_INTERNAL_H_

#include <libmm-glib.h>

struct cm_modem;

void cm_modem_obj_set_mm_modem_object(struct cm_modem *modem,
				      MMObject *mm_modemobj,
				      cm_err_t *err);

#endif /* _CM_MANAGER_MM_INTERNAL_H_ */

