/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef _THLIST_H_
#define _THLIST_H_

#include "ttypes.h"

T_BEGIN_DECLS

typedef struct _thlist_head thlist_head;
typedef struct _thlist_node thlist_node;

struct _thlist_head
{
    thlist_node *first;
};

struct _thlist_node
{
    thlist_node *next;
    thlist_node **pprev;
};

#define t_hlist_init_head(head) ((head)->first = NULL)
#define t_hlist_init_node(node) \
{                               \
    (node)->next = NULL;        \
    (node)->pprev = NULL;       \
}
T_EXTERN void t_hlist_insert(thlist_head *head, thlist_node *node);
T_EXTERN void t_hlist_remove(thlist_node *node);
T_EXTERN tbool t_hlist_is_empty(const thlist_head *head);
T_EXTERN tuint32 t_hlist_length(const thlist_head *head);


/**
 * @brief get hlist node entry
 * @param member_ptr - node pointer
 * @param struct_type - entry struct type
 * @param member - node name
 */
#define t_hlist_entry(member_ptr, struct_type, member) \
        T_CONTAINER_OF(member_ptr, struct_type, member)

/**
 * @brief list foreach
 * @param pos - current node pointer
 * @param head - list head
 */
#define t_hlist_foreach(pos, head) \
        for (pos = (head)->first; pos != NULL; pos = pos->next)



T_END_DECLS

#endif /* _THASH_H_ */
