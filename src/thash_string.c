/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "thash_string.h"
#include "tassert.h"

/****************************************************
 * macros definition
 ****************************************************/
#define DEFAULT_TABLE_SIZE    (16)
#define REHASH_FACTOR         (1.2)

/****************************************************
 * struct definition
 ****************************************************/
struct _thash_string
{
    tuint32 table_size;
    /* rehash border */
    tuint32 max_element_size;
    tuint32 element_count;
    thlist_head *head;
};

/****************************************************
 * static variable 
 ****************************************************/

/****************************************************
 * functions 
 ****************************************************/
/**
 * @brief get string hash value
 * @param hash_string - hash table
 * @param key - string to hash
 * @return hash value
 */
static tuint32 t_hash_string_hash(const thash_string *hash_string, const char *key)
{
    tuint32 hash_val = 0;
    while (*key != '\0')
    {
        hash_val = (hash_val << 5) + *key++;
    }

    return hash_val & (hash_string->table_size - 1);

}

/**
 * @brief round data to the nearest interger
 * @param data - data to round
 * @return nearest interger
 */
static tint tround(tdouble data)
{
    if (data < 0)
    {
        return (tint)(data - 0.5);
    }
    else
    {
        return (tint)(data + 0.5);
    }
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
            tdouble max_size = REHASH_FACTOR * hash->table_size;
            hash->max_element_size = tround(max_size);
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
tint t_hash_string_init_node(thash_string_node *node, const char *key)
{
    T_ASSERT(NULL != node);
    
    if (NULL != key)
    {
        /* new key */
        node->key = malloc(strlen(key) + 1);
        if (NULL != node->key)
        {
            strcpy(node->key, key);
        }
        else
        {
            return -ENOMEM;
        }
    }
    t_hlist_init_node(&node->node);

    return 0;
}


/**
 * @brief new string hash node
 * @param key - key string
 * @return node pointer
 */
thash_string_node *t_hash_string_new_node(const tchar *key)
{
    thash_string_node *node = malloc(sizeof(thash_string_node));
    if (NULL != node)
    {
        if (NULL != key)
        {
            node->key = malloc(strlen(key) + 1);
            strcpy(node->key, key);
        }
        t_hlist_init_node(&node->node);
    }

    return node;
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
thash_string *t_hash_string_insert(thash_string *hash_string, thash_string_node *node)
{
    T_ASSERT(NULL != hash_string);
    T_ASSERT(NULL != node);
    T_ASSERT(NULL != node->key);

    tuint32 hash_val = t_hash_string_hash(hash_string, node->key);
    //check if already contain
    thlist_node *hlist_node = NULL;
    thash_string_node *string_node = NULL;

    t_hlist_foreach(hlist_node, &hash_string->head[hash_val])
    {
        string_node = t_hlist_entry(hlist_node, thash_string_node, node);
        if (0 == strcmp(string_node->key, node->key))
        {
            return hash_string;
        }
    }

    //insert node
    t_hlist_insert(&hash_string->head[hash_val], &node->node);
    hash_string->element_count++;

    //check if need rehash 
    if (hash_string->element_count > hash_string->max_element_size)
    {
        thash_string *new_hash_string = t_hash_string_rehash(hash_string, hash_string->table_size * 2);
        if (NULL != new_hash_string)
        {
            //delete old hash string
            free(hash_string->head); 
            free(hash_string);
            return new_hash_string;
        }
    }

    return hash_string;
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
 * @brief get hash node which key euqals to key in hash table
 * @param hash_string - hash table
 * @param key - key to find
 * @return hash node
 */
thash_string_node *t_hash_string_get(const thash_string *hash_string, const char *key)
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
            return string_node;
        }
    }

    return NULL;
}


/**
 * @brief get hash table all keys
 * @param hash_string - string hash table
 * @param keys - output key buffers
 */
void t_hash_string_keys(const thash_string *hash_string, char **keys)
{
    T_ASSERT(NULL != hash_string);
    T_ASSERT(NULL != keys);

    thlist_node *hlist_node = NULL;
    thash_string_node *string_node = NULL;


    tuint32 index = 0;
    for (tuint32 i = 0; i < hash_string->table_size; ++i)
    {
        t_hlist_foreach(hlist_node, &hash_string->head[i])
        {
            string_node = t_hlist_entry(hlist_node, thash_string_node, node);
            strcpy(keys[index], string_node->key);
            index ++;
        }
    }
    keys[index] = NULL;
}

/**
 * @brief iterator all node in hash table
 * @param hash_string - hash table handle
 * @param hash_func - hash callback function
 * @return error code, 0 means no error happend
 */
tint t_hash_string_foreach(const thash_string *hash_string, thash_func hash_func, void *userdata)
{
    if (NULL == hash_string)
    {
        return 0;
    }

    thlist_node *hlist_node = NULL;
    thash_string_node *string_node = NULL;
    tint err = 0;

    for (tuint32 i = 0; i < hash_string->table_size; ++i)
    {
        t_hlist_foreach(hlist_node, &hash_string->head[i])
        {
            string_node = t_hlist_entry(hlist_node, thash_string_node, node);
            if (NULL != hash_func)
            {
                err = hash_func(string_node, userdata);
                if (0 != err)
                {
                    return err;
                }
            }
        }
    }

    return 0;
}

/**
 * @brief check if there is any node whose key equals to key in hash table
 * @param hash_string - hash table
 * @param key - key to find
 * @return check value TRUE: contain FALSE: can't find
 */
tbool t_hash_string_contain(const thash_string *hash_string, const char *key)
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
 * @brief get hash table element count
 * @param hash_string - hash table pointer
 * @return hash table element count
 */
tuint32 t_hash_string_count(const thash_string *hash_string)
{
    T_ASSERT(NULL != hash_string);

    return hash_string->element_count;
}

/**
 * @brief get hash table table size
 * @param hash_string - hash table pointer
 * @return hash table table size
 */
tuint32 t_hash_string_capacity(const thash_string *hash_string)
{
    T_ASSERT(NULL != hash_string);

    return hash_string->table_size;
}

/**
 * @brief free all node in hash table 
 * @param hash_string - hash table 
 * @param free_func - resource free function
 */
void t_hash_string_free(thash_string *hash_string, tfree_func free_func)
{
    T_ASSERT(NULL != hash_string);

    thlist_head *head = NULL;
    for (tuint32 i = 0; i < hash_string->table_size; ++i)
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

    free(hash_string->head);
    free(hash_string);
}


/**
 * @brief free all node in hash table
 * @param hash_string - hash table 
 * @param free_func - resource free function
 */
void t_hash_string_clear(thash_string *hash_string, tfree_func free_func)
{
    T_ASSERT(NULL != hash_string);

    thlist_head *head = NULL;
    /* free node first */
    for (tuint32 i = 0; i < hash_string->table_size; ++i)
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

    /* free head */
    for (tuint32 i = DEFAULT_TABLE_SIZE; i < hash_string->table_size; ++i)
    {
        free(&hash_string->head[i]);
    }

    /* init head */
    for (tuint32 i = 0; i < hash_string->table_size; ++i)
    {
        t_hlist_init_head(&hash_string->head[i]);
    }

    hash_string->table_size = DEFAULT_TABLE_SIZE;
    hash_string->element_count = 0;
    tdouble max_size = REHASH_FACTOR * hash_string->table_size;
    hash_string->max_element_size = tround(max_size);
}


