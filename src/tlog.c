/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "../include/tlog/tlog.h"
#include "ttypes.h"
#include "tassert.h"
#include "tkeyfile.h"
#include "thash_string.h"
#include "tslist.h"
#include "tstring.h"
#include "level.h"
#include "format.h"
#include "rules.h"
#include "category.h"
#include "global.h"

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
    tslist node;
}mdc_list_node;

/****************************************************
 * static variable 
 ****************************************************/
/* key = format name, value = format */
static thash_string *formats_kv = NULL;
/* key = category name, value = category_node */
static thash_string *category_detail = NULL;
/* mdc */
static tslist *mdc = NULL;

/* default configure file */
static const tchar default_cfg[] = "[general]\n[format]\n[rules]\n*.*=>stdout";

/****************************************************
 * functions 
 ****************************************************/
/**
 * @brief filter general group
 * @param keyfile - keyfile handle
 * @return error code, 0 means no error
 */
static tint filter_group_general(tkeyfile *keyfile)
{
    T_ASSERT(NULL != keyfile);

    tint err = 0;
    if (t_keyfile_contains_group(keyfile, GROUP_NAME_GENRAL))
    {
    }

    return err;
}

/**
 * @brief filter configure file and construct memory 
 *        configure hash table
 * @param keyfile - keyfile handle
 * @return error code, 0 means no error
 */
static tint filter_config_file(tkeyfile *keyfile)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL == category_detail);
    tint err = 0;

    category_detail = t_hash_string_new();
    if (NULL == category_detail)
    {
        return -ENOMEM;
    }

    formats_kv = format_new();
    if (NULL == formats_kv)
    {
        return -ENOMEM;
    }

    err = filter_group_general(keyfile);
    if (0 != err)
    {
        return err;
    }

    err = filter_format(keyfile, &formats_kv);
    if (0 != err)
    {
        return err;
    }

    err = filter_rules(keyfile, formats_kv, &category_detail);
    if (0 != err)
    {
        return err;
    }

    return 0;
}

/**
 * @brief init tlog
 * @param keyfile - keyfile handle
 * @return error code, 0 means success
 */
tint tlog_internal_init(tkeyfile *keyfile)
{
    T_ASSERT(NULL != keyfile);

    /* filter configure file */
    return filter_config_file(keyfile);
}

/**
 * @brief init tlog environment from configure data
 * @param name - configure data
 * @param source - name source
 * @return error code, 0 means no error
 */
tint tlog_open(const tchar *name, tlog_source source)
{
    if ((NULL != category_detail) || (NULL != mdc))
    {
        return -EEXIST;
    }

    tkeyfile *keyfile = t_keyfile_new();
    if (NULL == keyfile)
    {
        return -ENOMEM;
    }

    /* use last character to seperate key-value */
    t_keyfile_use_last_sep(keyfile, TRUE);

    tint ret = 0;
    switch(source)
    {
    case TLOG_FILE:
        if (NULL == name)
        {
            ret = -EINVAL;
        }
        else
        {
            /* load configure file */
            ret = t_keyfile_load_from_file(keyfile, name);
        }
        break;
    case TLOG_MEM:
        if (NULL == name)
        {
            ret = -EINVAL;
        }
        else
        {
            ret = t_keyfile_load_from_data(keyfile, name);
        }
        break;
    case TLOG_DEFAULT:
    default:
        ret = t_keyfile_load_from_data(keyfile, default_cfg);
        break;
    }

    if (0 == ret)
    {
        ret = tlog_internal_init(keyfile);
        if (0 == ret)
        {
            mdc = malloc(sizeof(tslist));
            if (NULL == mdc)
            {
                tlog_close();
                return -ENOMEM;
            }
            else
            {
                t_slist_init_head(mdc);
            }
        }
    }

    /* free keyfile */
    t_keyfile_free(keyfile);
    
    return ret;
}

/**
 * @brief close tlog
 */
void tlog_close()
{
    if (NULL != category_detail)
    {
        category_free(category_detail);
        category_detail = NULL;
    }

    if (NULL != formats_kv)
    {
        format_free(formats_kv);
        formats_kv = NULL;
    }

    if (NULL != mdc)
    {
        tlog_clean_mdc();
    }
}

/**
 * @brief get category named 'name'
 * @return category handle
 */
const tlog_category *tlog_get_category(const tchar *name)
{
    if ((NULL == name) || (NULL == category_detail))
    {
        return NULL;
    }

    return get_category(category_detail, name);
}

/**
 * @brief log real output function
 * @param cat - log category handle
 * @param file - file name
 * @param line - log generate line
 * @param func - function name
 * @param level - log level
 * @param fmt - user log message
 */
void tlog(const tlog_category *cat, const char *file,
        long line, const char *func, const tchar *line_str,
        int level, const char *fmt, ...)
{
    if (NULL != cat)
    {
        tchar user_msg[256] = {0};
        va_list args;
        va_start(args, fmt);
        vsprintf(user_msg, fmt, args);
        category_gen_log(cat, file, line, func, line_str, level, user_msg);
        va_end(args);
    }
}

/**
 * @brief put mdc key-value to hash table
 * @param hash - hash table
 * @param key - key string
 * @param value - value string
 * @return error code, 0 means no error
 */
static tint tlog_put_mdc_node(thash_string **hash, const tchar *key, const tchar *value)
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
 * @param key - key string
 * @param value - value string
 * @return error code, 0 means no error
 */
tint tlog_put_mdc(const tchar *key, const tchar *value)
{
    if ((NULL == key) || (NULL == value))
    {
        return -EINVAL;
    }

    pthread_t tid = pthread_self();
    tslist *tmp_node;
    mdc_list_node *list_node;
    mdc_hash_node *hash_node;
    t_slist_foreach(tmp_node, mdc)
    {
        list_node = t_slist_entry(tmp_node, mdc_list_node, node);
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
                return tlog_put_mdc_node(&list_node->mdc_hash, key, value);
            }
        }
    }

    /* can not find mdc node */
    list_node = malloc(sizeof(mdc_list_node));
    if (NULL == list_node)
    {
        return -ENOMEM;
    }

    t_slist_init_node(&list_node->node);

    list_node->mdc_hash = t_hash_string_new();
    if (NULL == list_node->mdc_hash)
    {
        free(list_node);
        return -ENOMEM;
    }

    t_slist_append(mdc, &list_node->node);

    list_node->tid = tid;
    tint err = tlog_put_mdc_node(&list_node->mdc_hash, key, value);
    if (0 != err)
    {
        t_slist_remove(mdc, &list_node->node);
        free(list_node->mdc_hash);
        free(list_node);
    }

    return err;
}

/**
 * @brief get mdc value
 * @param key - key string
 * @return value string
 */
tchar *tlog_get_mdc(const tchar *key)
{
    if ((NULL == key) || (NULL == mdc))
    {
        return NULL;
    }

    pthread_t tid = pthread_self();
    tslist *tmp_node;
    mdc_list_node *list_node;
    mdc_hash_node *hash_node;
    t_slist_foreach(tmp_node, mdc)
    {
        list_node = t_slist_entry(tmp_node, mdc_list_node, node);
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
 * @param key - key string
 */
void tlog_remove_mdc(const tchar *key)
{
    if ((NULL == key) || (NULL == mdc))
    {
        return ;
    }

    pthread_t tid = pthread_self();
    tslist *tmp_node;
    mdc_list_node *list_node;
    mdc_hash_node *hash_node;
    t_slist_foreach(tmp_node, mdc)
    {
        list_node = t_slist_entry(tmp_node, mdc_list_node, node);
        /* find list node */
        if (pthread_equal(list_node->tid, tid))
        {
            thash_string_node *string_node = t_hash_string_get(list_node->mdc_hash, key);
            if (NULL != string_node)
            {
                t_hash_string_remove_node(string_node);
                hash_node = t_hash_string_entry(string_node, mdc_hash_node, node);
                free(hash_node->node.key);
                free(hash_node->value);
                free(hash_node);
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
    tslist *list_node = (tslist *)data;
    mdc_list_node *mdc_list = t_slist_entry(list_node, mdc_list_node, node);
    mdc_hash_free(mdc_list->mdc_hash);
    free(mdc_list);
}

/**
 * @brief clean all mdc key-value
 */
void tlog_clean_mdc(void)
{
    if (NULL != mdc)
    {
        t_slist_free(mdc, mdc_list_free);
        free(mdc);
        mdc = NULL;
    }
}


