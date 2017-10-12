#include "tkeyfile.h"
#include "thash_string.h"
#include "tslist.h"
#include "tassert.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "tstring.h"
#include "global.h"
#include "level.h"
#include "format.h"
#include "category.h"



/****************************************************
 * struct definition
 ****************************************************/
/* category detail */
typedef struct 
{
    tlog_level level;
    const char *format;
    const split_format *splits;
    char *output;
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
static tint category_free(void *data, void *userdata)
{
    T_ASSERT(NULL != data);
    thash_string_node *string_node = (thash_string_node *)data;
    category_node *cat_node = t_hash_string_entry(string_node, category_node, node);
    free(cat_node->category.name);
    free(cat_node->category.rules);
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
        }
        else
        {
            free(cat_node->category.name);
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
                    t_hash_string_foreach(*hash, category_free, NULL);
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
    cat_rule->format = get_format(format_hash, format);
    if (NULL == cat_rule->format)
    {
        cat_rule->format = DEFAULT_FORMAT;
        format = DEFAULT_FORMAT_NAME;
    }
    cat_rule->splits = get_format_split(format_hash, format);

    /* add output */
    cat_rule->output = malloc(strlen(output) + 1);;
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
        return NULL;
    }

    category_node *category = t_hash_string_entry(string_node, category_node, node);
    return &category->category;
}

void category_gen_log(const tlog_category *cat, const tchar *file,
        tint line, const tchar *func, const tchar *line_str,
        tuint32 level, const tchar *fmt, va_list args)
{
    T_ASSERT(NULL != cat);
    tchar user_msg[256];
    tchar msg_buf[512];
    vsprintf(user_msg, fmt, args);
    preprocess_info pre = {file, func, line, line_str, level, user_msg};
    tuint32 count = 0;
    for (tuint32 i = 0; i < cat->count; ++i)
    {
        if (0 != ((cat->rules[i].level & level) & LEVEL_MASK))
        {
            count = format_split_to_string(msg_buf, cat->rules[i].splits, &pre);
            /* for test only */
            write(0, msg_buf, count);
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


