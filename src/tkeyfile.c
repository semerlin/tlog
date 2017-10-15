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
#include "tkeyfile.h"
#include "tassert.h"
#include "tshareptr.h"
#include "thash_string.h"
#include "tlist.h"
#include "tstring.h"


/****************************************************
 * macros definition
 ****************************************************/

/****************************************************
 * struct definition
 ****************************************************/
/* key-value node */
typedef struct
{
    tchar *value;
    thash_string_node node;
}kv_node;

/* group node */
typedef struct
{
    thash_string *kv;
    tchar *group_name;
    tlist node;
}group_node;

/* keyfile structure */
struct _tkeyfile
{
    tbool use_last_sep;
    tshareptr ptr;
    tlist groups;
};

/* key-value foreach userdata */
typedef struct
{
    tkey_value_func kv_func;
    void *userdata;
}kv_userdata;

/****************************************************
 * static variable
 ****************************************************/

/****************************************************
 * functions
 ****************************************************/
/**
 * @brief keyfile destroy callback function
 * @param data - keyfile handle
 */
static void t_keyfile_destroy(void *data)
{
    t_keyfile_free((tkeyfile *)data);
}

/**
 * @brief new keyfile
 * @return keyfile pointer
 */
tkeyfile *t_keyfile_new()
{
    tkeyfile *keyfile = (tkeyfile *)malloc(sizeof(tkeyfile));
    if (NULL != keyfile)
    {
        keyfile->use_last_sep = FALSE;
        t_shareptr_init((tshareptr *)keyfile, t_keyfile_destroy, keyfile);
        t_list_init_head(&keyfile->groups);
    }
    return keyfile;
}

/**
 * @brief free hash table node
 * @param data - hash table node
 */
static void t_keyfile_free_hash_node(void *data)
{
    thash_string_node *hash_node = (thash_string_node *)data;
    kv_node *kv = t_hash_string_entry(hash_node, kv_node, node);
    free(kv->value);
    free(kv);
}

/**
 * @brief free list node
 * @param data - list node
 */
static void t_keyfile_free_group_node(void *data)
{
    tlist *list = (tlist *)data;
    group_node *group = t_list_entry(list, group_node, node);
    if (NULL != group->kv)
    {
        t_hash_string_free(group->kv, t_keyfile_free_hash_node);
    }
    free(group->group_name);
    free(group);
}

/**
 * @brief free keyfile resource and keyfile handle
 * @param file - keyfile handle
 */
void t_keyfile_free(tkeyfile *file)
{
    T_ASSERT(NULL != file);
    t_list_free(&file->groups, t_keyfile_free_group_node);
    free(file);
}

/**
 * @brief free keyfile resource except keyfile
 * @param file - keyfile handle
 */
static void t_keyfile_free_internal(tkeyfile *file)
{
    t_list_free(&file->groups, t_keyfile_free_group_node);
    t_list_init_head(&file->groups);
}

/**
 * @brief find group in keyfile
 * @param keyfile - keyfile handle
 * @param name - group name
 */
static group_node *t_keyfile_find_group(const tkeyfile *keyfile, const tchar *name)
{
    tlist *node;
    group_node *group;
    t_list_foreach(node, &keyfile->groups) 
    {
        group = t_list_entry(node, group_node, node);
        if (0 == strcmp(group->group_name, name))
        {
            return group;
        }
    }

    return NULL;
}

/**
 * @brief new group node
 * @param name - group node name
 * @return group node pointer
 */
static group_node *t_keyfile_new_group_node(const tchar *name)
{
    group_node *node = malloc(sizeof(group_node));
    if (NULL != node)
    {
        node->group_name = malloc(strlen(name) + 1);
        if (NULL != node->group_name)
        {
            strcpy(node->group_name, name);
            t_list_init_node(&node->node);
            node->kv = NULL;
        }
        else
        {
            free(node);
            node = NULL;
        }
    }

    return node;
}

/**
 * @brief find group in string
 * @param data - parse string
 * @param group - output group name
 * @return TRUE: find group
 *         FALSE: no group find
 */
static tbool t_keyfile_parse_group(const tchar *data, tchar *group)
{
    if (*data != '[')
    {
        return FALSE;
    }
    tint end_index = t_string_find_char_reverse(data, 0, ']', TRUE);
    if (-1 != end_index)
    {
        if (1 < end_index)
        {
            tchar temp[512];
            t_string_mid(data, 1, end_index - 1, group);
            t_string_trimmed(group, temp);
            strcpy(group, temp);
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * @brief find key and value in string
 * @param keyfile - keyfile handle
 * @param data - trimmed parse string
 * @param key - output key name
 * @param value - output value name
 * @return TRUE: find key and value
 *         FALSE: no key or value find
 */
static tbool t_keyfile_parse_key_value(const tkeyfile *keyfile, const tchar *data, 
        tchar *key, tchar *value)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != data);
    T_ASSERT(NULL != key);
    T_ASSERT(NULL != value);

    tint index = -1;
    if (keyfile->use_last_sep)
    {
        index = t_string_find_char_reverse(data, 0, '=', TRUE);
    }
    else
    {
        index = t_string_find_char(data, 0, '=', TRUE);
    }

    if (-1 != index)
    {
        tchar temp[512];
        //get key
        t_string_left(data, index, key);
        t_string_trimmed_tail(key, temp);
        if (0 == strlen(key))
        {
            return FALSE;
        }
        strcpy(key, temp);
        //get value
        t_string_right(data, strlen(data) - index - 1, value);
        t_string_trimmed_head(value, temp);
        strcpy(value, temp);
        t_string_remove_linebreak(value, temp);
        strcpy(value, temp);
        return TRUE;
    }

    return FALSE;
}


/**
 * @brief use last '=' to seperate key and value
 * @param keyfile - keyfile handle
 * @param TRUE: use
 *        FALSE: usr first '='
 */
void t_keyfile_use_last_sep(tkeyfile *keyfile, tbool flag)
{
    keyfile->use_last_sep = flag;
}

/**
 * @brief parse line data ti keyfile
 * @param keyfile - keyfile handle
 * @param line - line data
 */
static tint t_keyfile_load_line(tkeyfile *keyfile, const tchar *line, group_node **cur_group, tbool *process_kv)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != line);
    T_ASSERT(NULL != process_kv);

    char trimmed[512];
    char key[512];
    char value[512];

    kv_node *keyvalue_node = NULL;
    thash_string_node *hash_node = NULL;

    t_string_trimmed(line, trimmed);
    if ('#' == trimmed[0]) /* comment check */
    {
        return 0;
    }
    else if (t_keyfile_parse_group(trimmed, key)) /* find group */
    {
        //check if already has this names group
        group_node *node = t_keyfile_find_group(keyfile, key);
        if (NULL == node)
        {
            //new group
            node = t_keyfile_new_group_node(key);
            if (NULL == node)
            {
                t_keyfile_free_internal(keyfile);
                return -ENOMEM;
            }
            t_list_append(&keyfile->groups, &node->node);
        }
        *cur_group = node;
        *process_kv = TRUE;
    }
    else if(t_keyfile_parse_key_value(keyfile, trimmed, key, value))
    {
        if (*process_kv)
        {
            if (NULL == (*cur_group)->kv)
            {
                (*cur_group)->kv = t_hash_string_new();
                if (NULL == (*cur_group)->kv)
                {
                    t_keyfile_free_internal(keyfile);
                    return -ENOMEM;
                }
            }
            //check if already has this key
            hash_node = t_hash_string_get((*cur_group)->kv, key);
            if (NULL == hash_node)
            {
                keyvalue_node = malloc(sizeof(kv_node));
                if (NULL == keyvalue_node)
                {
                    t_keyfile_free_internal(keyfile);
                    return -ENOMEM;
                }
                keyvalue_node->node.key = malloc(strlen(key) + 1);
                keyvalue_node->value = malloc(strlen(value) + 1);
                
                if ((NULL == keyvalue_node->node.key) ||
                    (NULL == keyvalue_node->value))
                {
                    t_keyfile_free_internal(keyfile);
                    return -ENOMEM;
                }

                strcpy(keyvalue_node->node.key, key);
                strcpy(keyvalue_node->value, value);
                (*cur_group)->kv = t_hash_string_insert((*cur_group)->kv, &keyvalue_node->node);
            }
        }
    }

    return 0;
}

/**
 * @brief load keyfile from file
 * @param keyfile - keyfile handle
 * @param file - configure file name
 * @return if success return 0 otherwise return errno
 */
tint t_keyfile_load_from_file(tkeyfile *keyfile, const tchar *file)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != file);

    FILE *fp = fopen(file, "r");
    if (NULL == fp)
    {
        return -errno;
    }

    tchar line_buf[513];
    tchar *line_str = NULL;
    tbool process_keyvalue = FALSE;
    group_node *cur_group = NULL;
    tint ret = 0;
    while (1)
    {
        line_str = fgets(line_buf, 512, fp);
        if (NULL == line_str)
        {
            break;
        }
        line_str[512] = '\0';

        ret = t_keyfile_load_line(keyfile, line_str, &cur_group, &process_keyvalue);
        if (0 != ret)
        {
            break;
        }
    }

    return ret;
}

/**
 * @brief load keyfile from memory buffer
 * @param keyfile - keyfile handle
 * @param buf - memory buffer
 * @return error code, 0 means no error happend
 */
tint t_keyfile_load_from_data(tkeyfile *keyfile, const tchar *buf)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != buf);

    tchar line_buf[513];
    tbool process_keyvalue = FALSE;
    group_node *cur_group = NULL;
    tint ret = 0;
    tint len;
    tuint32 index = 0;
    while (1)
    {
        len = t_string_get_line(line_buf, buf, 512, index);
        if (-1 == len)
        {
            break;
        }

        ret = t_keyfile_load_line(keyfile, line_buf, &cur_group, &process_keyvalue);
        if (0 != ret)
        {
            break;
        }
        index += len + 1;
    }

    return ret;
}

/**
 * @brief find key-value node by key
 * @param keyfile - keyfile handle
 * @param group - key-value group name
 * @param key - key name
 */
static kv_node *t_keyfile_find_kv(const tkeyfile *keyfile, const tchar *group,
        const tchar *key)
{
    group_node *group_n = t_keyfile_find_group(keyfile, group);
    if (NULL != group_n)
    {
        thash_string_node *hash_node = t_hash_string_get(group_n->kv, key);
        if (NULL != hash_node)
        {
            kv_node *kv = t_hash_string_entry(hash_node, kv_node, node);
            return kv;
        }
    }

    return NULL;
}

/**
 * @brief get string value from keyfile
 * @param group - string group
 * @param key - string key
 * @param out - output string value
 * @param default_value - if not find key, fill out with default_value
 */
void t_keyfile_get_string(const tkeyfile *keyfile, const tchar *group, 
        const tchar *key, tchar *out, const tchar *default_value)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != group);
    T_ASSERT(NULL != key);
    T_ASSERT(NULL != out);

    kv_node *kv = t_keyfile_find_kv(keyfile, group, key);
    if (NULL != kv)
    {
        strcpy(out, kv->value);
    }
    else
    {
        strcpy(out, default_value);
    }
}

/**
 * @brief get int value from keyfile
 * @param group - int group
 * @param key - int key
 * @param default_value - if not find key, fill out with default_value
 * @return key value
 */
tint t_keyfile_get_int(const tkeyfile *keyfile, const tchar *group, 
        const tchar *key, const tint default_value)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != group);
    T_ASSERT(NULL != key);

    kv_node *kv = t_keyfile_find_kv(keyfile, group, key);
    if (NULL != kv)
    {
        tint temp = 0;
        if (t_string_to_int(kv->value, &temp))
        {
            return temp;
        }

        return default_value;
    }

    return default_value;
}

/**
 * @brief get bool value from keyfile
 * @param group - bool group
 * @param key - bool key
 * @param default_value - if not find key, fill out with default_value
 * @return key value
 */
tbool t_keyfile_get_bool(const tkeyfile *keyfile, const tchar *group, 
        const tchar *key, const tbool default_value)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != group);
    T_ASSERT(NULL != key);

    kv_node *kv = t_keyfile_find_kv(keyfile, group, key);
    if (NULL != kv)
    {
        tint temp = 0;
        if (t_string_to_bool(kv->value, &temp))
        {
            return temp;
        }

        return default_value;
    }

    return default_value;
}

/**
 * @brief hash table node iterator callback
 * @param data - hash table node
 * @para userdata - user data
 */
static tint hash_string_foreach(void *data, void *userdata)
{
    T_ASSERT(NULL != data);
    T_ASSERT(NULL != userdata);

    kv_userdata *kv_data = (kv_userdata *)userdata;
    thash_string_node *string_node = data;
    kv_node *kv = t_hash_string_entry(string_node, kv_node, node);
    tint err = 0;
    if (NULL != kv_data->kv_func)
    {
        err = kv_data->kv_func(kv->node.key, kv->value, kv_data->userdata);
    }
    return err;
}

/**
 * @brief get keyfile group hash table
 * @param keyfile - keyfile handle
 * @param group_name - group name
 * @param kv_func - callback function
 * @return error code, 0 means no error happend
 */
tint t_keyfile_group_foreach(const tkeyfile *keyfile, const tchar *group_name,
        tkey_value_func kv_func, void *userdata)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != group_name);
    tlist *list_node;
    group_node *group = NULL;
    tint err = 0;
    t_list_foreach(list_node, &keyfile->groups) 
    {
        group = t_list_entry(list_node, group_node, node);
        if (0 == strcmp(group->group_name, group_name))
        {
           break;
        }
    }

    if (NULL != group)
    {
        kv_userdata data = {kv_func, userdata};
        err = t_hash_string_foreach(group->kv, hash_string_foreach, &data);
    }

    return err;
}

/**
 * @brief get keyfile all group names
 * @param keyfile - keyfile handle
 * @param groups - groups output buffer
 */
void t_keyfile_group_names(const tkeyfile *keyfile, tchar **groups)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != groups);
    tlist *node;
    group_node *group;
    tuint32 index = 0;
    t_list_foreach(node, &keyfile->groups) 
    {
        group = t_list_entry(node, group_node, node);
        strcpy(groups[index], group->group_name);
        index++;
    }
    groups[index] = NULL;
}

/**
 * @brief get keyfile specific group's keys
 * @param keyfile - keyfile handle
 * @param group - group name
 * @param keys - keys output buffer
 */
void t_keyfile_key_names(const tkeyfile *keyfile, const tchar *group_name, tchar **keys)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != group_name);
    T_ASSERT(NULL != keys);

    tlist *node;
    group_node *group;
    t_list_foreach(node, &keyfile->groups) 
    {
        group = t_list_entry(node, group_node, node);
        if (0 == strcmp(group->group_name, group_name))
        {
            break;
        }
        group = NULL;
    }

    if (NULL != group)
    {
        t_hash_string_keys(group->kv, keys);
    }
    else
    {
        keys[0] = NULL;
    }
}

/**
 * @brief get keyfile group count
 * @param keyfile - keyfile handle
 * @return keyfile group count
 */
tuint32 t_keyfile_group_count(const tkeyfile *keyfile)
{
    T_ASSERT(NULL != keyfile);
    return t_list_length(&keyfile->groups);
}

/**
 * @brief get keyfile key count in specific group
 * @param keyfile - keyfile handle
 * @param group_name - group name
 * @return keyfile key count in specific group
 */
tuint32 t_keyfile_key_count(const tkeyfile *keyfile, const tchar *group_name)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != group_name);

    tlist *node;
    group_node *group;
    t_list_foreach(node, &keyfile->groups) 
    {
        group = t_list_entry(node, group_node, node);
        if (0 == strcmp(group->group_name, group_name))
        {
            break;
        }
        group = NULL;
    }

    if (NULL != group)
    {
        return t_hash_string_count(group->kv);
    }
    else
    {
        return 0;
    }

}

/**
 * @brief check if keyfile has specified group
 * @param keyfile - keyfile handle
 * @param group - group name
 * @return TRUE: has
 *         FALSE: can not find
 */
tbool t_keyfile_contains_group(const tkeyfile *keyfile, const tchar *group)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != group);
    return (NULL != t_keyfile_find_group(keyfile, group));
}

/**
 * @brief check if keyfile has specified key in group
 * @param keyfile - keyfile handle
 * @param group - group name
 * @param key - key name
 * @return TRUE: has
 *         FALSE: can not find
 */
tbool t_keyfile_contains_key(const tkeyfile *keyfile, const tchar *group, const tchar *key)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != group);
    T_ASSERT(NULL != key);

    group_node *group_n = t_keyfile_find_group(keyfile, group);
    if (NULL != group_n)
    {
        thash_string_node *hash_node = t_hash_string_get(group_n->kv, key);
        if (NULL != hash_node)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @brief reference keyfile use
 * @param keyfile - keyfile handle
 * @retunr keyfile handle
 */
tkeyfile *t_keyfile_ref(tkeyfile *keyfile)
{
    T_ASSERT(NULL != keyfile);
    return (tkeyfile *)t_shareptr_ref((tshareptr *)keyfile);
}
/**
 * @brief unreference keyfile use
 * @param keyfile - keyfile handle
 */
void t_keyfile_unref(tkeyfile **keyfile)
{
    T_ASSERT(NULL != keyfile);
    t_shareptr_unref((tshareptr **)keyfile);
}


