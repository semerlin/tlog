#include "tslist.h"
#include "tassert.h"



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
 * @brief prepend node to list
 * @param head - list head
 * @param node - node to prepend
 */
void t_slist_prepend(tslist *head, tlist *node)
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
void t_slist_append(tslist *head, tlist *node)
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
void t_slist_remove(tslist *head, tlist *node)
{
    T_ASSERT(NULL != head);
    T_ASSERT(NULL != node);

    tslist *prev, *next;
    for (prev = head, next = head->next; next != head; 
            prev = next, next = next->next)
    {
        if (next == node)
        {
            prev->next = next->next;
            break;
        }
    }
}

/**
 * @brief remove all node equal to node from list
 * @param head - list head
 * @param node - node to remove
 */
void t_slist_remove_all(tslist *head, tlist *node)
{
    T_ASSERT(NULL != head);
    T_ASSERT(NULL != node);

    tslist *prev, *next;
    for (prev = head, next = head->next; next != head; 
            prev = next, next = next->next)
    {
        if (next == node)
        {
            prev->next = next->next;
        }
    }
}

/**
 * @brief judge if list is empty
 * @param head - list head
 * @return TRUE - empty
 *         FALSE - not empty
 */
tbool t_slist_is_empty(tslist *head)
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
tbool t_slist_is_last(tslist *head, tlist *node)
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
tbool t_slist_is_first(tslist *head, tlist *node)
{
    T_ASSERT(NULL != head);

    return (head->next == node);
}

/**
 * @brief get list length
 * @param head - list head
 * @return list length
 */
tint t_slist_legth(tslist *head)
{
    T_ASSERT(NULL != head);

    tint length = 0;
    tslist *temp = head;
    for (; temp->next != head; temp = temp->next)
    {
        ++length;
    }

    return length;
}



