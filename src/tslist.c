/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "tslist.h"
#include "tassert.h"


/****************************************************
 * macros definition
 ****************************************************/

/****************************************************
 * struct definition
 ****************************************************/

/****************************************************
 * static variable 
 ****************************************************/

/****************************************************
 * functions 
 ****************************************************/
/**
 * @brief init list head
 * @param head - list head pointer
 */
void t_slist_init_head(tslist *head)
{
    T_ASSERT(NULL != head);
    head->next = head;
}

/**
 * @brief init list node
 * @param node - list node pointer
 */
void t_slist_init_node(tslist *node)
{
    T_ASSERT(NULL != node);
    node->next = NULL;
}

/**
 * @brief prepend node to list
 * @param head - list head
 * @param node - node to prepend
 */
void t_slist_prepend(tslist *head, tslist *node)
{
    T_ASSERT(NULL != head);
    T_ASSERT(NULL != node);

    node->next = head->next;
    head->next = node;
}

/**
 * @brief append node to list
 * @param head - list head
 * @param node - node to append
 */
void t_slist_append(tslist *head, tslist *node)
{
    T_ASSERT(NULL != head);
    T_ASSERT(NULL != node);

    tslist *last_node = head;
    for (; last_node->next != head; last_node = last_node->next);
    last_node->next = node;
    node->next = head;
}

/**
 * @brief remove first node equal to node from list
 * @param head - list head
 * @param node - node to remove
 */
void t_slist_remove(tslist *head, tslist *node)
{
    T_ASSERT(NULL != head);
    T_ASSERT(NULL != node);

    tslist *cur = head->next;
    tslist *prev = head;
    for (; cur != head; prev = cur, cur = cur->next)
    {
        if (cur == node)
        {
            prev->next = cur->next;
            node->next = NULL;
            break;
        }
    }
}

/**
 * @brief judge if list is empty
 * @param head - list head
 * @return TRUE - empty
 *         FALSE - not empty
 */
tbool t_slist_is_empty(const tslist *head)
{
    T_ASSERT(NULL != head);

    return (head->next == head);
}

/**
 * @brief judge if node is the last node
 * @param head - list head
 * @param node - node to judgement
 * @return TRUE - yes
 *         FALSE - no
 */
tbool t_slist_is_last(const tslist *head, const tslist *node)
{
    T_ASSERT(NULL != head);

    return (node->next == head);
}


/**
 * @brief judge if node is the first node
 * @param head - list head
 * @param node - node to judgement
 * @return TRUE - yes
 *         FALSE - no
 */
tbool t_slist_is_first(const tslist *head, const tslist *node)
{
    T_ASSERT(NULL != head);

    return (head->next == node);
}

/**
 * @brief get list length
 * @param head - list head
 * @return list length
 */
tuint32 t_slist_length(const tslist *head)
{
    T_ASSERT(NULL != head);

    tint length = 0;
    tslist *temp = head->next;
    for (; temp != head; temp = temp->next)
    {
        ++length;
    }

    return length;
}


/**
 * @brief free all node in list except head
 * @param head - list head
 * @param free_func - resource free function
 */
void t_slist_free(tslist *head, tfree_func free_func)
{
    T_ASSERT(NULL != head);

    tslist *cur = head->next;
    tslist *temp = NULL;
    while (cur != head)
    {
        temp = cur->next;
        if (NULL != free_func)
        {
            free_func(cur);
        }
        cur = temp;
    }
}

