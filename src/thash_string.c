#include "thash_string.h"
#include "tassert.h"
#include <string.h>
#include <stdlib.h>

#define DEFAULT_HASH_SIZE    (20)

struct _thash_string
{
    tuint32 table_size;
    tuint32 element_count;
    thlist_head *head;
};

/**
 * @brief get string hash value
 * @param hash_string - hash table
 * @param key - string to hash
 * @return hash value
 */
static tuint32 t_hash_string_hash(const thash_string *hash_string, const char *key)
{
    T_ASSERT(NULL != key);
    T_ASSERT(NULL != hash_string);
    
    tuint32 hash_val = 0;
    while (*key != '\0')
    {
        hash_val = (hash_val << 5) + *key++;
    }

    return hash_val % hash_string->table_size;

}


/**
 * @brief create new hash table
 * @return if create success return hash table pointer otherwise return NULL
 */
thash_string *t_hash_string_new(void)
{
    thash_string *hash = malloc(sizeof(thash_string));
    if (NULL != hash)
    {
        hash->head = calloc(sizeof(thlist_head), DEFAULT_HASH_SIZE);
        if (NULL != hash->head)
        {
            hash->table_size = DEFAULT_HASH_SIZE;
            hash->element_count = 0;
            tuint32 i = 0;
            for (; i < DEFAULT_HASH_SIZE; ++i)
            {
                t_hlist_init_head(&hash->head[i]);
            }
        }
        else
        {
            free(hash);
            hash = NULL;
        }
    }

    return hash;
}

/**
 * @brief insert hash node to hash table
 * @param hash_string - hash table
 * @param node - node to insert
 */
void t_hash_string_insert(thash_string *hash_string, thash_string_node *node)
{
    T_ASSERT(NULL != hash_string);
    T_ASSERT(NULL != key);
    T_ASSERT(NULL != node);

    tuint32 hash_val = t_hash_string_hash(hash_string, node->key);
    //check if already contain
    thlist_node *hlist_node = NULL;
    thash_string_node *string_node = NULL;

    t_hlist_foreach(hlist_node, &hash_string->head[hash_val])
    {
        string_node = t_hlist_entry(hlist_node, thash_string_node, node);
        if (0 == strcmp(string_node->key, node->key))
        {
            return ;
        }
    }

    //insert node
    t_hlist_insert(&hash_string->head[hash_val], &node->node);
    hash_string->element_count++;

    //TODO rehash 
}

/**
 * @brief remove node key equals to key from hash table
 * @param hash_string - hash table
 * @param key - key to remove
 * @return removed hash node pointer
 */
thash_string_node *t_hash_string_remove(thash_string *hash_string, const char *key)
{
    T_ASSERT(NULL != hash_string);
    T_ASSERT(NULL != key);

    thash_string_node *string_node = t_hash_string_get(hash_string, key);
    if (NULL != string_node)
    {
        t_hlist_remove(&(string_node->node));
        hash_string->element_count--;
    }

    return string_node;
}


/**
 * @brief remove node from hash table
 * @param node - node to remove
 */
void t_hash_string_remove_node(thash_string_node *node)
{
    T_ASSERT(NULL != node);

    t_hlist_remove(&node->node);
}

/**
 * @brief get hash node which key euqals to key in hash table
 * @param hash_string - hash table
 * @param key - key to find
 * @return hash node
 */
thash_string_node *t_hash_string_get(thash_string *hash_string, const char *key)
{
    T_ASSERT(NULL != hash_string);
    T_ASSERT(NULL != key);

    tuint32 hash_val = t_hash_string_hash(hash_string, key);
    thlist_node *hlist_node = NULL;
    thash_string_node *string_node = NULL;

    t_hlist_foreach(hlist_node, &hash_string->head[hash_val])
    {
        string_node = t_hlist_entry(hlist_node, thash_string_node, node);
        if (0 == strcmp(string_node->key, key))
        {
            break; 
        }
    }

    return string_node;
}

/**
 * @brief check if there is any node whose key equals to key in hash table
 * @param hash_string - hash table
 * @param key - key to find
 * @return check value TRUE: contain FALSE: can't find
 */
tbool t_hash_string_contain(thash_string *hash_string, const char *key)
{
    T_ASSERT(NULL != hash_string);
    T_ASSERT(NULL != key);

    tuint32 hash_val = t_hash_string_hash(hash_string, key);
    thlist_node *hlist_node = NULL;
    thash_string_node *string_node = NULL;

    t_hlist_foreach(hlist_node, &hash_string->head[hash_val])
    {
        string_node = t_hlist_entry(hlist_node, thash_string_node, node);
        if (0 == strcmp(string_node->key, key))
        {
            return TRUE;
        }
    }

    return FALSE;
}


/**
 * @brief free all node in hash table 
 * @param hash_string - hash table 
 * @param free_func - resource free function
 */
void t_hash_string_free(thash_string *hash_string, tfree_callback free_func)
{
    T_ASSERT(NULL != head);
    T_ASSERT(NULL != free_func);

    thlist_head *head = NULL;
    tuint32 i = 0;
    for (; i < hash_string->table_size; ++i)
    {
        head = &hash_string->head[i];

        thlist_node *cur = head->first;
        thlist_node *temp = NULL;
        while (cur != NULL)
        {
            temp = cur->next;
            free_func(t_hlist_entry(cur, thash_string_node, node));
            cur = temp;
        }
    }
}


