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
        head->first->pprev = &node;
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
    (*(node->pprev))->next = node->next;
    node->next->pprev = node->pprev;
    node->next = NULL;
    node->pprev = NULL;
}



