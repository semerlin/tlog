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
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "tkeyfile.h"
#include "thash_string.h"
#include "tslist.h"
#include "tassert.h"
#include "tstring.h"
#include "global.h"
#include "level.h"
#include "format.h"
#include "category.h"

/****************************************************
 * macros definition
 ****************************************************/

/****************************************************
 * struct definition
 ****************************************************/
/* category detail */
typedef struct 
{
    tlog_level level;
    const tchar *format;
    const split_format *splits;
    tchar *output;
    FILE* fd;
}category_rule;

/* category */
struct _tlog_category
{
    tchar *name;
    tuint32 count;
    category_rule *rules;
};

/* category node */
typedef struct
{
    tlog_category category;
    thash_string_node node;
}category_node;

/* name node */
typedef struct
{
    tchar *name;
    tuint32 count;
    tslist node;
}category_name_node;

/****************************************************
 * static variable 
 ****************************************************/

/****************************************************
 * functions 
 ****************************************************/
/**
 * @brief free name list 
 * @param data - node to free
 */
static void free_name_list(void *data)
{
    T_ASSERT(NULL != data);
    category_name_node *name_node = t_slist_entry((tslist *)data, 
            category_name_node, node);
    free(name_node->name);
    free(name_node);
}

/**
 * @brief statistics category name count
 * @param name - category name 
 * @return 0 success, otherwise failed
 */
static tint name_count(void *key, void *value, void *userdata)
{
    T_ASSERT(NULL != key);
    T_ASSERT(NULL != userdata);

    tslist *head = (tslist *)userdata;

    /* split category name */
    tint index = t_string_find_char((const tchar *)key, 0, '.', TRUE);
    tchar buf[256];
    tchar name[256];
    if (-1 != index)
    {
        t_string_left(key, index, buf);
        t_string_trimmed(buf, name);
    }
    else
    {
        t_string_trimmed(key, name);
    }

    tslist *cat_node;
    category_name_node *name_node;
    t_slist_foreach(cat_node, head)
    {
        name_node = t_slist_entry(cat_node, category_name_node, node); 
        if (0 == strcmp(name_node->name, name))
        {
            /* find same name, so increase count */
            name_node->count ++;
            return 0;
        }
    }

    /* add new node */
    name_node = malloc(sizeof(category_name_node));
    if (NULL != name_node)
    {
        name_node->name = malloc(strlen(name) + 1);
        if (NULL == name_node->name)
        {
            free(name_node);
            return -ENOMEM;
        }
        strcpy(name_node->name, name);
        name_node->count = 1;
        t_slist_init_node(&name_node->node);
        t_slist_append(head, &name_node->node);
    }
    else
    {
        return -ENOMEM;
    }

    return 0;
}

/**
 * @brief free category hash table
 * @param data - category hash node 
 * @param userdata - userdata
 */
static tint category_free_internal(void *data, void *userdata)
{
    T_ASSERT(NULL != data);
    thash_string_node *string_node = (thash_string_node *)data;
    category_node *cat_node = t_hash_string_entry(string_node, category_node, node);
    free(cat_node->category.name);
    for (tuint32 i = 0; i < cat_node->category.count; ++i)
    {
        if (NULL != cat_node->category.rules[i].fd)
        {
            fclose(cat_node->category.rules[i].fd);
            free(cat_node->category.rules[i].output);
        }
    }
    free(cat_node->category.rules);
    free(string_node->key);
    free(cat_node);
    return 0;
}

/**
 * @brief add category node to hash table
 * @param hash - hash table handle
 * @param name - category name
 * @param count - category count
 */
static category_node *add_category_node(thash_string **hash, const tchar *name, tuint32 count)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);
    T_ASSERT(count > 0);

    category_node *cat_node = malloc(sizeof(category_node));
    if (NULL != cat_node)
    {
        cat_node->category.name = malloc(strlen(name) + 1);
        if (NULL == cat_node->category.name)
        {
            free(cat_node);
            return NULL;
        }
        strcpy(cat_node->category.name, name);

        /* alloc rules array */
        cat_node->category.rules = calloc(sizeof(category_rule), count);
        if (NULL == cat_node->category.rules)
        {
            free(cat_node->category.name);
            free(cat_node);
            return NULL;
        }

        if (0 == t_hash_string_init_node(&cat_node->node, name))
        {
            *hash = t_hash_string_insert(*hash, &cat_node->node);
            cat_node->category.count = 0;
            for (tuint32 i = 0; i < count; ++i)
            {
                cat_node->category.rules[i].level = TLOG_DEBUG;
                cat_node->category.rules[i].format = NULL;
                cat_node->category.rules[i].splits = NULL;
                cat_node->category.rules[i].output = NULL;
                cat_node->category.rules[i].fd = NULL;
            }
        }
        else
        {
            free(cat_node->category.name);
            free(cat_node->category.rules);
            free(cat_node);
            return NULL;
        }
    }

    return cat_node;
}

/**
 * @brief init category space
 * @param keyfile - keyfile handle
 * @param hash - category hash table
 */
tint categories_init(const tkeyfile *keyfile, thash_string **hash)
{
    tint err = 0;
    tslist name_head;
    t_slist_init_head(&name_head);
    /* statistics name count */
    err = t_keyfile_group_foreach(keyfile, GROUP_NAME_RULES, name_count, &name_head);
    if (0 == err)
    {
        /* alloc spaces */
        tslist *name_node_temp = NULL;
        category_name_node *name_node;
        category_node *cat_node = NULL;
        if (0 == t_slist_length(&name_head))
        {
            /* add default category */
            cat_node = add_category_node(hash, DEFAULT_CATEGORY_NAME, 1);
            if (NULL == cat_node)
            {
                return -ENOMEM;
            }
        }
        else
        {
            t_slist_foreach(name_node_temp, &name_head)
            {
                name_node = t_slist_entry(name_node_temp, category_name_node, node); 
                cat_node = add_category_node(hash, name_node->name, name_node->count);
                if (NULL == cat_node)
                {
                    t_slist_free(&name_head, free_name_list);
                    t_hash_string_foreach(*hash, category_free_internal, NULL);
                    return -ENOMEM;
                }
            }

            /* free key list */
            t_slist_free(&name_head, free_name_list);
        }
    }

    return err;
}


/**
 * @brief free category memory
 * @param cat_hash - category hash table
 */
void category_free(thash_string *cat_hash)
{
    T_ASSERT(NULL != cat_hash);
    t_hash_string_foreach(cat_hash, category_free_internal, NULL);
    free(cat_hash);
}

/**
 * @brief validation output format string
 * @param output - output format string
 * @return validation status
 */
static tbool output_format_validation(const tchar *output)
{
    T_ASSERT(NULL != output);
    tint ret = TRUE;
    if ('>' == *output)
    {
        if ((0 == strcmp(">stdout", output)) ||
            (0 == strcmp(">stderr", output)))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if('|' == *output)
    {
        output ++;
        while(' ' == *output)
        {
            output++;
        }
        if ('\0' == *output)
        {
            return FALSE;
        }

        return TRUE;
    }
    else
    {
        tuint32 out_index = 0;

        while ('\0' != output[out_index])
        {
            if ('%' != output[out_index])
            {
                out_index ++;
            }
            else
            {
                out_index ++;
                switch (output[out_index])
                {
                case 'd':
                {
                    out_index ++;
                    if ('(' == output[out_index])
                    {
                        out_index ++;
                        tint index = t_string_find_char(output, out_index, ')', TRUE);
                        if (-1 != index)
                        {
                            out_index += index + 1;
                        }
                        else
                        {
                            ret = FALSE;
                        }
                    }
                    else
                    {
                        ret = FALSE;
                    }
                }
                    break;
                default:
                    ret = FALSE;
                    break;
                }
            }

            if (!ret)
            {
                break;
            }
        }
    }

    return ret;
}

/**
 * @brief convert output name
 * @param name - name output
 * @param output - output format
 * @return error code, 0 means no error
 */
static tint output_convert_quick(tchar *name, const tchar *output)
{
    T_ASSERT(NULL != name);
    T_ASSERT(NULL != output);

    tint name_index = 0, out_index = 0;

    while ('\0' != output[out_index])
    {
        if ('%' != output[out_index])
        {
            name[name_index] = output[out_index];
            name_index ++;
            out_index ++;
        }
        else
        {
            out_index += 3;
            tint index = t_string_find_char(output, out_index, ')', TRUE);
            tchar time_str[32];
            strncpy(time_str, output + out_index, index);
            time_str[index] = '\0';
            out_index += index + 1;
            time_t tm = time(NULL);
            struct tm ltm;
            if (NULL != localtime_r(&tm, &ltm))
            {
                name_index += strftime(name + name_index, 99, time_str, &ltm);
            }
        }
    }

    name[name_index] = '\0';
    return 0;
}

/**
 * @brief get output file handle
 * @param output - output string
 * @return file handle, -1 means error happend
 */
static FILE *get_output_fd(const tchar *output)
{
    T_ASSERT(NULL != output);

    if (output_format_validation(output))
    {
        if (0 == strcmp(">stdout", output))
        {
            return stdout;
        }
        else if(0 == strcmp(">stderr", output))
        {
            return stderr;
        }
        else if('|' == *output)
        {
            output ++;
            /* pipeline */
            while(' ' == *output)
            {
                output++;
            }
            return popen(output, "w");
        }
        else
        {
            /* file */
            tchar filename[256];
            if (0 == output_convert_quick(filename, output))
            {
                return fopen(filename, "a");
            }
            else
            {
                errno = -EINVAL;
                return NULL;
            }
        }
    }

    errno = -EINVAL;
    return NULL;
}

/**
 * @brief add level value to hash table
 * @param cat_hash - category hash table handle
 * @param format_hash - format hash table
 * @param name - category name 
 * @param level - level string
 * @param format - format string
 * @param output - output string
 */
tint add_category(thash_string *cat_hash, const thash_string *format_hash, 
        const tchar *name, const tchar *level, 
        const tchar *format, const tchar *output)
{
    T_ASSERT(NULL != cat_hash);
    T_ASSERT(NULL != name);
    T_ASSERT(NULL != format_hash);

    thash_string_node *string_node = t_hash_string_get(cat_hash, name);
    if (NULL == string_node)
    {
        return -EINVAL;
    }
    category_node *cat_node = t_hash_string_entry(string_node, category_node, node);

    category_rule *cat_rule = &cat_node->category.rules[cat_node->category.count];
    /* add level */
    cat_rule->level = log_level_convert(level);

    /* add format */
    if (0 == strcmp("", format))
    {
        format = DEFAULT_FORMAT_NAME;
    }
    cat_rule->format = get_format(format_hash, format);
    T_ASSERT(NULL != cat_rule->format);
    if (NULL == cat_rule->format)
    {
        return -EINVAL;
        //cat_rule->format = DEFAULT_FORMAT;
        //format = DEFAULT_FORMAT_NAME;
    }
    cat_rule->splits = get_format_split(format_hash, format);

    /* add output */
    tuint32 out_len = 0;
    if (0 == strcmp(output, ""))
    {
        out_len = strlen(DEFAULT_OUTPUT);
    }
    else
    {
        out_len = strlen(output);
    }
    cat_rule->output = malloc(out_len + 1);;
    if (NULL != cat_rule->output)
    {
        if (0 == strcmp(output, ""))
        {
            strcpy(cat_rule->output, DEFAULT_OUTPUT);
        }
        else
        {
            strcpy(cat_rule->output, output);
        }

        cat_rule->fd = get_output_fd(output);
        if (NULL == cat_rule->fd)
        {
            return errno;
        }
    }
    else
    {
        return -ENOMEM;
    }

    cat_node->category.count++;

    return 0;
}


/**
 * @brief get category named 'name'
 * @return category handle
 */
tlog_category *get_category(const thash_string *hash, const tchar *name)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);

    thash_string_node *string_node = t_hash_string_get(hash, name);
    if (NULL == string_node)
    {
        string_node = t_hash_string_get(hash, "*");
        if (NULL == string_node)
        {
            return NULL;
        }
    }

    category_node *category = t_hash_string_entry(string_node, category_node, node);
    return &category->category;
}

/**
 * @brief generate log message
 * @param cat - category handle
 * @param file - file name
 * @param line - line
 * @param func - function name
 * @param line_str - line string
 * @param level - level 
 * @param fmt - user message
 */
void category_gen_log(const tlog_category *cat, const tchar *file,
        tlong line, const tchar *func, const tchar *line_str,
        tuint32 level, const tchar *msg)
{
    T_ASSERT(NULL != cat);
    tchar msg_buf[512];
    preprocess_info pre = {file, func, line, line_str, level, msg};
    tuint32 count = 0;
    for (tuint32 i = 0; i < cat->count; ++i)
    {
        if (0 != ((cat->rules[i].level & level) & LEVEL_MASK))
        {
            count = format_split_to_string(msg_buf, cat->rules[i].splits, &pre);
            fwrite(msg_buf, 1, count, cat->rules[i].fd);
        }
    }
}



/**
 * @brief print category infomation to stdout
 * @param data - hash node
 * @param userdata - userdata
 */
static tint category_print(void *data, void *userdata)
{
    category_node *category = t_hash_string_entry((thash_string_node *)data, category_node, node);
    printf("category = %s(%d)\n", category->category.name, category->category.count);
    for (tuint32 i = 0; i < category->category.count; ++i)
    {
        printf("  level = 0x%x\n", category->category.rules[i].level);
        printf("  format = %s\n", category->category.rules[i].format);
        printf("  output = %s\n", category->category.rules[i].output);
        printf("\n");
    }
    return 0;
}

/**
 * @brief print category infomation to stdout
 * @param hash - category hash table
 */
void print_category(const thash_string *hash)
{
    T_ASSERT(NULL != hash);
    t_hash_string_foreach(hash, category_print, NULL);
}


