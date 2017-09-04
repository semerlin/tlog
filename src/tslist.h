#ifndef _TSLIST_H_
#define _TSLIST_H_

#include "ttypes.h"

T_BEGIN_DECLS


typedef _tslist tslist;

struct _tslist
{
    tslist *next;
};


T_EXTERN void t_slist_init_head(tlist *head)
T_EXTERN void t_slist_prepend(tlist *head, tlist *node)
T_EXTERN void t_slist_append(tlist *head, tlist *node)
T_EXTERN void t_slist_remove(tlist *head, tlist *node)
T_EXTERN void t_slist_remove_all(tlist *head, tlist *node)
T_EXTERN tbool t_slist_is_empty(tlist *head)
T_EXTERN tbool t_slist_is_last(tlist *head, tlist *node)
T_EXTERN tbool t_slist_is_first(tlist *head, tlist *node)
T_EXTERN tint t_slist_legth(tlist *head)

/**
 * @brief get list node entry
 * @param member_ptr - node pointer
 * @param struct_type - entry struct type
 * @param member - node name
 */
#define t_slist_entry(member_ptr, struct_type, member) \
        T_CONTAINER_OF(member_ptr, struct_type, member)

/**
 * @brief list
 * @param pos - current node pointer
 * @param head - list head
 */
#define t_slist_foreach(pos, head) \
        for (pos = (head)->next; pos != (head); pos = pos->next)


T_END_DECLS




#endif
