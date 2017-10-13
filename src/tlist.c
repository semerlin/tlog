/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "tlist.h"
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
 * @param head - list head
 */
void t_list_init_head(tlist *head)
{
    T_ASSERT(NULL != head);
    head->prev = head;
    head->next = head;
}

/**
 * @brief init list node
 * @param node - list node
 */
void t_list_init_node(tlist *node)
{
    T_ASSERT(NULL != node);
    node->prev = NULL;
    node->next = NULL;
}

/**
 * @brief prepend node to list
 * @param head - list head
 * @param node - node to prepend
 */
void t_list_prepend(tlist *head, tlist *node)
{
    T_ASSERT(NULL != head);
    T_ASSERT(NULL != node);

    head->next->prev = node;
    node->next = head->next;
    head->next = node;
    node->prev = head;
}

/**
 * @brief append node to list
 * @param head - list head
 * @param node - node to append
 */
void t_list_append(tlist *head, tlist *node)
{
    T_ASSERT(NULL != head);
    T_ASSERT(NULL != node);

    head->prev->next = node;
    node->prev = head->prev;
    head->prev = node;
    node->next = head;
}

/**
 * @brief remove node from list
 * @param node - node to remove
 */
void t_list_remove(tlist *node)
{
    T_ASSERT(NULL != node->prev);
    T_ASSERT(NULL != node->next);

    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;
}

/**
 * @brief judge if list is empty
 * @param head - list head
 * @return TRUE - empty
 *         FALSE - not empty
 */
tbool t_list_is_empty(const tlist *head)
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
tbool t_list_is_last(const tlist *head, const tlist *node)
{
    T_ASSERT(NULL != head);

    return (head->prev == node);
}


/**
 * @brief judge if node is the first node
 * @param head - list head
 * @param node - node to judgement
 * @return TRUE - yes
 *         FALSE - no
 */
tbool t_list_is_first(const tlist *head, const tlist *node)
{
    T_ASSERT(NULL != head);

    return (head->next == node);
}

/**
 * @brief get list length
 * @param head - list head
 * @return list length
 */
tuint32 t_list_length(const tlist *head)
{
    T_ASSERT(NULL != head);

    tint length = 0;
    tlist *temp = head->next;
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
void t_list_free(tlist *head, tfree_func free_func)
{
    T_ASSERT(NULL != head);

    tlist *cur = head->next;
    tlist *temp = NULL;
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


