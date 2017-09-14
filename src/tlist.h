#ifndef _TLIST_H_
#define _TLIST_H_

#include "ttypes.h"
#include "tcommon.h"

T_BEGIN_DECLS


typedef struct _tlist tlist;

struct _tlist
{
    tlist *prev;
    tlist *next;
};


T_EXTERN void t_list_init_head(tlist *head);
T_EXTERN void t_list_init_node(tlist *node);
T_EXTERN void t_list_prepend(tlist *head, tlist *node);
T_EXTERN void t_list_append(tlist *head, tlist *node);
T_EXTERN void t_list_remove(tlist *node);
T_EXTERN tbool t_list_is_empty(tlist *head);
T_EXTERN tbool t_list_is_last(tlist *head, tlist *node);
T_EXTERN tbool t_list_is_first(tlist *head, tlist *node);
T_EXTERN tint t_list_length(tlist *head);
T_EXTERN void t_list_free(tlist *head, tfree_callback free_func);

/**
 * @brief get list node entry
 * @param member_ptr - node pointer
 * @param struct_type - entry struct type
 * @param member - node name
 */
#define t_list_entry(member_ptr, struct_type, member) \
        T_CONTAINER_OF(member_ptr, struct_type, member)

/**
 * @brief list
 * @param pos - current node pointer
 * @param head - list head
 */
#define t_list_foreach(pos, head) \
        for (pos = (head)->next; pos != (head); pos = pos->next)


T_END_DECLS




#endif
