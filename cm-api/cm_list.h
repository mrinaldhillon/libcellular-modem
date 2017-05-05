#ifndef _CM_LIST_H_
#define _CM_LIST_H_

/*
 * ccan is c code archive.
 * Its a collection of c code snippets for linux, freebsd
 * and other sources.
 */

#include "ccan/list/list.h"

/* Embedded list functions mapping */

#define cm_list_head			list_head
#define cm_list_node			list_node

#define CM_LIST_HEAD_INIT		LIST_HEAD_INIT
#define cm_list_head_init		list_head_init
#define cm_list_node_init		list_node_init

#define cm_list_add			list_add
#define cm_list_add_tail		list_add_tail
#define cm_list_empty			list_empty
#define cm_list_del			list_del
#define cm_list_del_init		list_del_init

#define cm_list_entry			list_entry
#define cm_list_top			list_top
#define cm_list_next			list_next
#define cm_list_tail			list_tail

#define cm_list_for_each		list_for_each
#define cm_list_for_each_safe		list_for_each_safe
#define cm_list_for_each_rev		list_for_each_rev
#define cm_list_for_each_rev_safe	list_for_each_rev_safe
/*
struct cm_list {
	struct _cm_list_head head;
};

struct cm_list_node {
	void *data;
	struct _cm_list_node;
};
*/

#endif /* _CM_LIST_H_ */
