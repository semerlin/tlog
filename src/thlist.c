#include "thlist.h"
#include "tassert.h"


/**
 * @brief insert node to hash list
 * @param head - hash list head
 * @param node - node to insert
 */
void t_hlist_insert(thlist_head *head, thlist_node *node)
{
    T_ASSERT(NULL != head);
    T_ASSERT(NULL != node);

    node->next = head->first;
    if (NULL != head->first)
    {
        head->first->pprev = &node->next;
    }
    head->first = node;
    node->pprev = &head->first;
}

/**
 * @brief remove node from hash list
 * @param node - node to remove
 */
void t_hlist_remove(thlist_node *node)
{
    T_ASSERT(NULL != node);
    thlist_node **pprev = node->pprev;
    *(node->pprev) = node->next;

    if (NULL != node->next)
    {
        node->next->pprev = pprev;
    }

    node->pprev = NULL;
    node->next = NULL;
}

/**
 * @brief judge if hash list is empty
 * @param head - list head
 * @return TRUE - empty
 *         FALSE - not empty
 */
tbool t_hlist_is_empty(thlist_head *head)
{
    T_ASSERT(NULL != head);
    
    return (NULL == head->first);
}


/**
 * @brief get hash list length
 * @param head - list head
 * @return list length
 */
tint t_hlist_length(thlist_head *head)
{
    T_ASSERT(NULL != head);

    thlist_node *node = head->first;
    tint length = 0;

    while (NULL != node)
    {
        length ++;
        node = node->next;
    }

    return length;
}





