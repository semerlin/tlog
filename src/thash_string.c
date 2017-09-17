#include "thash_string.h"
#include "tassert.h"
#include <string.h>
#include <stdlib.h>

#define DEFAULT_TABLE_SIZE    (20)

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
 * @param table_size - hash table size
 * @return if create success return hash table pointer otherwise return NULL
 */
static thash_string *t_hash_string_new_size(tuint32 table_size)
{
    thash_string *hash = malloc(sizeof(thash_string));
    if (NULL != hash)
    {
        hash->head = calloc(sizeof(thlist_head), table_size);
        if (NULL != hash->head)
        {
            hash->table_size = table_size;
            hash->element_count = 0;
            tuint32 i = 0;
            for (; i < table_size; ++i)
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
 * @brief rehash old hash table to new hash table
 * @param hash_string - old hash table
 * @param table_size - new hash table size
 * @return if success new hash table pointer return and delete old hash table,
 *         otherwise NULL return and no change to old hash table
 */
static thash_string *t_hash_string_rehash(thash_string *hash_string, tuint32 table_size)
{
    T_ASSERT(NULL != hash_string);

    if (table_size < DEFAULT_TABLE_SIZE)
    {
        table_size = DEFAULT_TABLE_SIZE;
    }

    thash_string *new_hash_string = t_hash_string_new_size(table_size);

    if (NULL != new_hash_string)
    {
        thlist_head *hlist_head = NULL;
        thlist_node *hlist_node = NULL, *hlist_node_next = NULL;
        thash_string_node *string_node = NULL;

        tuint32 index = 0;
        //foreach every entry in old hash table
        for (index = 0; index < hash_string->table_size; ++index)
        {
            hlist_head = &hash_string->head[index];
            hlist_node = hlist_head->first;

            //insert to new hash table
            while(hlist_node != NULL)
            {
                hlist_node_next = hlist_node->next;
                string_node = t_hlist_entry(hlist_node, thash_string_node, node);
                t_hash_string_insert(new_hash_string, string_node);
                hlist_node = hlist_node_next;
            }
        }
    }

    return new_hash_string;
}

/**
 * @brief init string hash node
 * @param node - string hash node
 * @param key - key string
 */
void t_hash_string_init_node(thash_string_node *node, const char *key)
{
    T_ASSERT(NULL != node);
    
    if (NULL != key)
    {
        node->key = malloc(strlen(key));
        strcpy((node)->key, key);
    }
    t_hlist_init_node(&node->node);
}


/**
 * @brief create new hash table
 * @return if create success return hash table pointer otherwise return NULL
 */
thash_string *t_hash_string_new(void)
{
    return t_hash_string_new_size(DEFAULT_TABLE_SIZE);
}

/**
 * @brief insert hash node to hash table
 * @param hash_string - hash table
 * @param node - node to insert
 */
void t_hash_string_insert(thash_string *hash_string, thash_string_node *node)
{
    T_ASSERT(NULL != hash_string);
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

    //check if need rehash 
    if (hash_string->element_count > (hash_string->table_size * 1.2))
    {
        t_hash_string_rehash(hash_string, hash_string->table_size * 2);
    }
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
    T_ASSERT(NULL != hash_string);

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
            if (NULL != free_func)
            {
                free_func(t_hlist_entry(cur, thash_string_node, node));
            }
            cur = temp;
        }
    }
}


