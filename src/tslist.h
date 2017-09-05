#ifndef _TSLIST_H_
#define _TSLIST_H_

#include "ttypes.h"
#include "tcommon.h"

T_BEGIN_DECLS


typedef struct _tslist tslist;

struct _tslist
{
    tslist *next;
};


T_EXTERN void t_slist_init_head(tslist *head);
T_EXTERN void t_slist_prepend(tslist *head, tslist *node);
T_EXTERN void t_slist_append(tslist *head, tslist *node);
T_EXTERN void t_slist_remove(tslist *head, tslist *node);
T_EXTERN void t_slist_remove_all(tslist *head, tslist *node);
T_EXTERN tbool t_slist_is_empty(tslist *head);
T_EXTERN tbool t_slist_is_last(tslist *head, tslist *node);
T_EXTERN tbool t_slist_is_first(tslist *head, tslist *node);
T_EXTERN tint t_slist_legth(tslist *head);
T_EXTERN void t_slist_free(tslist *head, tfree_callback free_func);

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
