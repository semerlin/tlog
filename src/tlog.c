#include "../include/tlog/tlog.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "ttypes.h"
#include "tassert.h"
#include "tkeyfile.h"
#include "thash_string.h"
#include <stdio.h>

/****************************************************
 * macros definition
 ****************************************************/
#define GROUP_NAME_GENRAL    "general"
#define GROUP_NAME_FORMAT    "format"
#define GROUP_NAME_RULES     "rules"

#define DEFAULT_OUTPUT           ">stdout"
#define DEFAULT_LEVEL            "*"
#define DEFAULT_FORMAT_NAME      "default"
#define DEFAULT_FORMAT           "[%t]"
#define DEFAULT_CATEGORY_NAME    "*"

/****************************************************
 * struct definition
 ****************************************************/
/* format node */
typedef struct
{
    tchar *value;
    thash_string_node node;
}format_node;

/* level map */
struct
{
    const tchar name[16];
    const tlog_level level;
}log_level_info[] = 
{
    {"debug", TLOG_DEBUG},
    {"info", TLOG_INFO},
    {"notice", TLOG_NOTICE},
    {"warn", TLOG_WARN},
    {"error", TLOG_ERROR},
    {"fatal", TLOG_FATAL},
    {"", TLOG_DEBUG | TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL}
};

/* category detail */
struct _tlog_category
{
    tlog_level level;
    const char *format;
    char *output;
};

/* category node */
typedef struct
{
    tlog_category category;
    thash_string_node node;
}category_node;

/****************************************************
 * static variable 
 ****************************************************/
/* key = format name, value = format */
static thash_string *format_group = NULL;
/* key = category name, value = category_node */
static thash_string *category_detail = NULL;


/****************************************************
 * functions 
 ****************************************************/

/**
 * @brief convert level string to level int value
 * @param level - level string
 * @return level int value
 */
static tuint32 log_level_convert(const tchar *level)
{
    T_ASSERT(NULL != level);

    const tchar *plevel = level;

    /* get level flag */
    tuint8 level_flag = 0;
    tuint8 start_index = 0;
    tuint8 info_count = sizeof(log_level_info) / sizeof(log_level_info[0]);

    if ('>' == *plevel)
    {
        plevel++;
        level_flag = 2;
        start_index = 1;
        if ('=' == *plevel)
        {
            level_flag++;
            start_index++;
        }
    }
    else if('=' == *plevel)
    {
        level_flag = 1;
        start_index = 1;
    }
    else if(('*' == plevel[0]) && ('\0' == plevel[1]))
    {
        return log_level_info[info_count - 1].level;
    }
    else if('\0' != *plevel)
    {
        level_flag = 3;
        start_index = 0;
    }
    else
    {
        return 0;
    }

    tuint32 level_value = 0;
    /* get level text */
    level += start_index;

    for (tuint8 i = 0; i < info_count; ++i)
    {
        if (0 == strcmp(log_level_info[i].name, level))
        {
            switch(level_flag)
            {
            case 1:
                /* "=" */
                level_value = log_level_info[i].level;
                break;
            case 2:
                /* ">" */
                level_value = log_level_info[info_count].level;
                level_value &= ~(((log_level_info[i].level - 1) << 1) + 1);
                break;
            case 3:
            default:
                /* ">=" */
                level_value = log_level_info[info_count].level;
                level_value &= ~(log_level_info[i].level - 1);
                break;
            }
            break;
        }
    }

    return level_value;
}

/**
 * @brief get format value from hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @return format value
 */
static const tchar *get_format(thash_string *hash, const tchar *name)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);

    thash_string_node *string_node = t_hash_string_get(hash, name);
    if (NULL == string_node)
    {
        return NULL;
    }

    format_node *node = t_hash_string_entry(string_node, format_node, node);

    return node->value;
}

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
 * @brief add format value to hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @param format - format string
 */
static tint add_format(thash_string *hash, const tchar *name, const tchar *format)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);
    T_ASSERT(NULL != format);

    tint err = 0;
    format_node *node = malloc(sizeof(format_node));
    if (NULL != node)
    {
        err = t_hash_string_init_node(&node->node, name);
        if (0 == err)
        {
            /* add format */
            node->value = malloc(strlen(format) + 1);;
            if (NULL != node->value)
            {
                strcpy(node->value, format);
            }
            else
            {
                free(node->node.key);
                free(node);
                err = -ENOMEM;
            }
            hash = t_hash_string_insert(hash, &node->node);
        }
    }
    else
    {
        err = -ENOMEM;
    }

    return err;
}

/**
 * @brief add level value to hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @param level - level string
 * @param format - format string
 * @param output - output string
 */
static tint add_category(thash_string *hash, const tchar *name, const tchar *level,
        const tchar *format, const tchar *output)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);

    tint err = 0;
    category_node *node = malloc(sizeof(category_node));
    if (NULL != node)
    {
        err = t_hash_string_init_node(&node->node, name);
        if (0 == err)
        {
            /* add level */
            node->category.level = log_level_convert(level);

            /* add format */
            node->category.format = get_format(format_group, format);
            if (NULL == node->category.format)
            {
                node->category.format = DEFAULT_FORMAT;
            }

            /* add output */
            node->category.output = malloc(strlen(output) + 1);;
            if (NULL != node->category.output)
            {
                if (0 == strcmp(output, ""))
                {
                    strcpy(node->category.output, DEFAULT_OUTPUT);
                }
                else
                {
                    strcpy(node->category.output, output);
                }
            }
            else
            {
                free(node->node.key);
                free(node);
                err = -ENOMEM;
            }
            hash = t_hash_string_insert(hash, &node->node);
        }
    }
    else
    {
        err = -ENOMEM;
    }

    return err;
}

/**
 * @brief insert key and value to format hash table
 * @param key - format name string
 * @param value - format string
 * @return error code, 0 means no error
 */
static tint format_process(void *key, void *value)
{
    return add_format(format_group, key, value);
}

/**
 * @brief split rules string to level, format and output
 * @param rules - rules string to split
 * @param level - level string output
 * @param format - format string output
 * @param output - ouput string output
 */
static void split_rules(const tchar *rules, tchar *level, tchar *format, tchar *output)
{
    T_ASSERT(NULL != rules);
    T_ASSERT(NULL != level);
    T_ASSERT(NULL != format);
    T_ASSERT(NULL != output);
}

/**
 * @brief process rules and insert to hash table
 * @param key - category name
 * @param value - rules
 * @return error code, 0 means no error
 */
/* level;format;output */
static tint rules_process(void *key, void *value)
{
    tchar level[256];
    tchar format[256];
    tchar output[256];
    split_rules((const tchar *)value, level, format, output);
    return add_category(category_detail, key, level, format, output);
}

/**
 * @brief filter output format group
 * @param keyfile - keyfile handle
 * @return error code, 0 means no error
 */
static tint filter_group_format(tkeyfile *keyfile)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != format_group);

    tint err = 0;

    /* check group exists first */
    if (t_keyfile_contains_group(keyfile, GROUP_NAME_FORMAT))
    {
        err = t_keyfile_group_foreach(keyfile, GROUP_NAME_FORMAT, format_process);
    }
    else
    {
        err = add_format(format_group, DEFAULT_FORMAT_NAME, DEFAULT_FORMAT);
    }

    if (0 == err)
    {
        /* add default format */
        if (!t_hash_string_contain(format_group, DEFAULT_FORMAT_NAME))
        {
            err = add_format(format_group, DEFAULT_FORMAT_NAME, DEFAULT_FORMAT);
        }
    }

    return err;
}

/* category_name=level;output;format */
static tint filter_group_rules(tkeyfile *keyfile)
{ 
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != category_detail);
    T_ASSERT(NULL != format_group);

    tint err = 0;
    /* check group exists first */
    if (t_keyfile_contains_group(keyfile, GROUP_NAME_RULES))
    {
        err = t_keyfile_group_foreach(keyfile, GROUP_NAME_RULES, rules_process);
    }
    else
    {
        const tchar *format = get_format(format_group, DEFAULT_FORMAT_NAME);
        if (NULL == format)
        {
            format = DEFAULT_FORMAT;
        }
 
        err = add_category(category_detail, DEFAULT_CATEGORY_NAME, DEFAULT_LEVEL,
                DEFAULT_FORMAT, DEFAULT_OUTPUT);
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
    tint err = 0;

    if ((NULL == category_detail) &&
        (NULL == format_group))
    {
        category_detail = t_hash_string_new();
        if (NULL == category_detail)
        {
            return -ENOMEM;
        }

        format_group = t_hash_string_new();
        if (NULL == format_group)
        {
            return -ENOMEM;
        }

        err = filter_group_general(keyfile);
        if (0 != err)
        {
            return err;
        }

        err = filter_group_format(keyfile);
        if (0 != err)
        {
            return err;
        }

        err = filter_group_rules(keyfile);
        if (0 != err)
        {
            return err;
        }
       
        if (0 == t_hash_string_count(category_detail))
        {
            const tchar *format = get_format(format_group, DEFAULT_FORMAT_NAME);
            if (NULL == format)
            {
                format = DEFAULT_FORMAT;
            }
     
            err = add_category(category_detail, DEFAULT_CATEGORY_NAME, DEFAULT_LEVEL,
                    DEFAULT_FORMAT, DEFAULT_OUTPUT);
        }
    }
    else
    {
        return -EEXIST;
    }

    return 0;
}

#if 1
static tint category_print(void *data)
{
    category_node *category = t_hash_string_entry((thash_string_node *)data, category_node, node);
    printf("category = %s\n", category->node.key);
    printf("  level = 0x%x\n", category->category.level);
    printf("  format = %s\n", category->category.format);
    printf("  output = %s\n", category->category.output);
    return 0;
}

static void print_category(void)
{
    t_hash_string_foreach(category_detail, category_print);
}

#endif

/**
 * @brief init tlog environment
 * @param cfg_file - configure file path
 * @return error code, 0 means no error
 */
int tlog_init(const char *cfg_file)
{
    /* if there is no config file then load default config */
    if (NULL == cfg_file)
    {
        return 0;
    }

    tkeyfile *keyfile = t_keyfile_new();
    if (NULL == keyfile)
    {
        return -ENOMEM;
    }

    /* load configure file */
    tint ret = t_keyfile_load_from_file(keyfile, cfg_file);
    if (0 != ret)
    {
        return ret;
    }

    /* filter configure file */
    ret = filter_config_file(keyfile);
    if (0 != ret)
    {
        return ret;
    }

    /* free keyfile */
    t_keyfile_free(keyfile);
    
    print_category();
    return 0;
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

    thash_string_node *string_node = t_hash_string_get(category_detail, name);
    category_node *category = t_hash_string_entry(string_node, category_node, node);
    if (NULL != category)
    {
        return &category->category;
    }
    else
    {
        return NULL;
    }
}
