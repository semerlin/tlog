/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "tassert.h"
#include "mdc.h"
#include "tlist.h"
#include "thash_string.h"


/****************************************************
 * macros definition
 ****************************************************/

/****************************************************
 * struct definition
 ****************************************************/
/* mdc hash node */
typedef struct
{
    tchar *value;
    thash_string_node node;
}mdc_hash_node;

/* mdc list */
typedef struct
{
    pthread_t tid;
    thash_string *mdc_hash;
    tlist node;
}mdc_list_node;


/****************************************************
 * static variable 
 ****************************************************/

/****************************************************
 * functions 
 ****************************************************/
/**
 * @brief new mdc struct
 * @return mdc handle
 */
mdc* mdc_new(void)
{
    mdc *pmdc = malloc(sizeof(tlist));
    if (NULL != pmdc)
    {
        t_list_init_head(pmdc);
    }
    return pmdc;
}

/**
 * @brief put mdc key-value to hash table
 * @param hash - hash table
 * @param key - key string
 * @param value - value string
 * @return error code, 0 means no error
 */
static tint mdc_put_node(thash_string **hash, const tchar *key, const tchar *value)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != key);
    T_ASSERT(NULL != value);

    /* new node */
    mdc_hash_node *hash_node = malloc(sizeof(mdc_hash_node));
    if (NULL != hash_node)
    {
        tint err = t_hash_string_init_node(&hash_node->node, key);
        if (0 != err)
        {
            free(hash_node);
            return err;
        }
        else
        {
            hash_node->value = malloc(strlen(value) + 1);
            if (NULL != hash_node->value)
            {
                strcpy(hash_node->value, value);
            }
            else
            {
                free(hash_node->node.key);
                free(hash_node);
                return -ENOMEM;
            }
        }
    }

    /* insert node */
    *hash = t_hash_string_insert(*hash, &hash_node->node);

    return 0;
}

/**
 * @brief put mdc key-value to hash table
 * @param pmdc - mdc handle
 * @param key - key string
 * @param value - value string
 * @return error code, 0 means no error
 */
tint mdc_put(mdc *pmdc, const tchar *key, const tchar *value)
{
    T_ASSERT(NULL != pmdc);
    T_ASSERT(NULL != key);
    T_ASSERT(NULL != value);

    pthread_t tid = pthread_self();
    tlist *tmp_node;
    mdc_list_node *list_node;
    mdc_hash_node *hash_node;
    t_list_foreach(tmp_node, pmdc)
    {
        list_node = t_list_entry(tmp_node, mdc_list_node, node);
        /* find list node */
        if (pthread_equal(list_node->tid, tid))
        {
            thash_string_node *string_node = t_hash_string_get(list_node->mdc_hash, key);
            if (NULL != string_node)
            {
                /* replace hash node value */
                hash_node = t_hash_string_entry(string_node, mdc_hash_node, node);
                if (0 != strcmp(hash_node->value, value))
                {
                    tchar *tmp_value = malloc(strlen(value) + 1);
                    if (NULL == tmp_value)
                    {
                        return -ENOMEM;
                    }
                    else
                    {
                        free(hash_node->value);
                        hash_node->value = tmp_value;
                        return 0;
                    }
                }
            }
            else
            {
                return mdc_put_node(&list_node->mdc_hash, key, value);
            }
        }
    }

    /* can not find mdc node */
    list_node = malloc(sizeof(mdc_list_node));
    if (NULL == list_node)
    {
        return -ENOMEM;
    }

    t_list_init_node(&list_node->node);

    list_node->mdc_hash = t_hash_string_new();
    if (NULL == list_node->mdc_hash)
    {
        free(list_node);
        return -ENOMEM;
    }

    t_list_append(pmdc, &list_node->node);

    list_node->tid = tid;
    tint err = mdc_put_node(&list_node->mdc_hash, key, value);
    if (0 != err)
    {
        t_list_remove(&list_node->node);
        free(list_node->mdc_hash);
        free(list_node);
    }

    return err;
}

/**
 * @brief get mdc value
 * @param pmdc - mdc handle
 * @param key - key string
 * @return value string
 */
tchar *mdc_get(const mdc *pmdc, const tchar *key)
{
    T_ASSERT(NULL != pmdc);
    T_ASSERT(NULL != key);

    pthread_t tid = pthread_self();
    tlist *tmp_node;
    mdc_list_node *list_node;
    mdc_hash_node *hash_node;
    t_list_foreach(tmp_node, pmdc)
    {
        list_node = t_list_entry(tmp_node, mdc_list_node, node);
        /* find list node */
        if (pthread_equal(list_node->tid, tid))
        {
            thash_string_node *string_node = t_hash_string_get(list_node->mdc_hash, key);
            if (NULL != string_node)
            {
                hash_node = t_hash_string_entry(string_node, mdc_hash_node, node);
                return hash_node->value;
            }
            else
            {
                return NULL;
            }
        }
    }

    /* can not find list node */
    return NULL;
}

/**
 * @brief remove mdc value
 * @param pmdc - mdc handle
 * @param key - key string
 */
void mdc_remove(mdc *pmdc, const tchar *key)
{
    T_ASSERT(NULL != pmdc);
    T_ASSERT(NULL != key);

    pthread_t tid = pthread_self();
    tlist *tmp_node;
    mdc_list_node *list_node;
    mdc_hash_node *hash_node;
    t_list_foreach(tmp_node, pmdc)
    {
        list_node = t_list_entry(tmp_node, mdc_list_node, node);
        /* find list node */
        if (pthread_equal(list_node->tid, tid))
        {
            thash_string_node *string_node = t_hash_string_remove(list_node->mdc_hash, key);
            if (NULL != string_node)
            {
                hash_node = t_hash_string_entry(string_node, mdc_hash_node, node);
                free(hash_node->node.key);
                free(hash_node->value);
                free(hash_node);

                if (0 == t_hash_string_count(list_node->mdc_hash))
                {
                    /* free list node */
                    t_list_remove(tmp_node);
                    free(list_node->mdc_hash);
                    free(list_node);
                }
            }
        }
    }
}


/**
 * @brief free mdc hash table
 * @param data - mdc node
 */
static void free_mdc_hash(void *data)
{
    T_ASSERT(NULL != data);
    thash_string_node *string_node = (thash_string_node *)data;
    mdc_hash_node *hash_node = t_hash_string_entry(string_node, mdc_hash_node, node);
    free(hash_node->node.key);
    free(hash_node->value);
    free(hash_node);
}

/**
 * @brief free mdc hash table
 * @param mdc - format hash table
 */
static void mdc_hash_free(thash_string *hash)
{
    T_ASSERT(NULL != hash);
    t_hash_string_free(hash, free_mdc_hash);
}

/**
 * @brief free mdc list
 * @param data - mdc list node
 */
static void mdc_list_free(void *data)
{
    T_ASSERT(NULL != data);
    tlist *list_node = (tlist *)data;
    mdc_list_node *mdc_list = t_list_entry(list_node, mdc_list_node, node);
    mdc_hash_free(mdc_list->mdc_hash);
    free(mdc_list);
}

/**
 * @brief clean all mdc key-value
 * @param pmdc - mdc handle
 */
void mdc_clean(mdc *pmdc)
{
    T_ASSERT(NULL != pmdc);
    t_list_free(pmdc, mdc_list_free);
}

/**
 * @brief free mdc struct
 * @param pmdc - mdc handle
 */
void mdc_free(mdc *pmdc)
{
    T_ASSERT(NULL != pmdc);
    t_list_free(pmdc, mdc_list_free);
    free(pmdc);
}
