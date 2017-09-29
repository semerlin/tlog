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
/* level node */
typedef struct
{
    tuint32 level;
    thash_string_node node;
}level_node;

/* string node */
struct _string_node
{
    tchar *value;
    thash_string_node node;
};

/* foramt */
typedef struct _string_node format_node;

/* output node */
typedef struct _string_node output_node;

struct
{
    const tchar name[16];
    const tlog_level level;
}log_level_info[] = 
{
    {"debug", TLOG_DEBUG},
    {"info", TLOG_INFO},
    {"warn", TLOG_WARN},
    {"error", TLOG_ERROR},
    {"fatal", TLOG_FATAL},
    {"", TLOG_DEBUG | TLOG_INFO | TLOG_WARN | TLOG_ERROR | TLOG_FATAL}
};

/****************************************************
 * static variable 
 ****************************************************/
/* key = format name, value = format */
static thash_string *format_group = NULL;
/* key = category name, value = level */
static thash_string *category_level = NULL;
/* key = category name, value = format */
static thash_string *category_format = NULL;
/* key = category name, value = output position */
static thash_string *category_output = NULL;


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
 * @brief add string value to hash table
 * @param hash - hash table handle
 * @param key - key string
 * @param value - value string
 */
static tint add_value(thash_string *hash, const tchar *key, const tchar *value)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != key);

    tint err = 0;
    struct _string_node *node = malloc(sizeof(struct _string_node));
    if (NULL != node)
    {
        err = t_hash_string_init_node(&node->node, key);
        if (0 == err)
        {
            node->value = malloc(strlen(value) + 1);;
            if (NULL != node->value)
            {
                strcpy(node->value, value);
                hash = t_hash_string_insert(hash, &node->node);
            }
            else
            {
                free(node->node.key);
                free(node);
                err = -ENOMEM;
            }
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
 * @param name -category name 
 * @param level - level string
 */
static tint add_level(thash_string *hash, const tchar *name, const tchar *level)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);
    T_ASSERT(NULL != level);

    tint err = 0;
    level_node *node = malloc(sizeof(level_node));
    if (NULL != node)
    {
        err = t_hash_string_init_node(&node->node, name);
        if (0 == err)
        {
            node->level = log_level_convert(level);
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
 * @brief add format value to hash table
 * @param hash - hash table handle
 * @param name -category name 
 * @param format - format string
 */
static tint add_format(thash_string *hash, const tchar *name, const tchar *format)
{
    return add_value(hash, name, format);
}

/**
 * @brief add output value to hash table
 * @param hash - hash table handle
 * @param name -category name 
 * @param output - output string
 */
static tint add_output(thash_string *hash, const tchar *name, const tchar *output)
{
    return add_value(hash, name, output);
}

/**
 * @brief get string value from hash table
 * @param hash - hash table handle
 * @param key - key value
 * @return value string
 */
static const tchar *get_value(thash_string *hash, const tchar *key)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != key);

    thash_string_node *string_node = t_hash_string_get(hash, key);
    if (NULL == string_node)
    {
        return NULL;
    }

    struct _string_node *node = t_hash_string_entry(string_node, struct _string_node, node);

    return node->value;
}

#if 0
/**
 * @brief get level value from hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @return level value
 */
static tuint32 get_level(thash_string *hash, const tchar *name)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);

    thash_string_node *string_node = t_hash_string_get(hash, name);
    if (NULL == string_node)
    {
        return 0;
    }

    level_node *node = t_hash_string_entry(string_node, level_node, node);

    return node->level;
}
#endif

/**
 * @brief get format value from hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @return format value
 */
static const tchar *get_format(thash_string *hash, const tchar *name)
{
    return get_value(hash, name);
}

#if 0
/**
 * @brief get output value from hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @return output value
 */
static const tchar *get_output(thash_string *hash, const tchar *name)
{
    return get_value(hash, name);
}
#endif


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
    tint err = 0;
    tchar level[256];
    tchar format[256];
    tchar output[256];
    split_rules((const tchar *)value, level, format, output);
    err = add_level(category_level, (const char *)key, level);
    if (0 != err)
    {
        return err;
    }

    const tchar *format_real = get_format(format_group, format);
    if (NULL == format_real)
    {
        format_real = DEFAULT_FORMAT;
    }
    err = add_format(category_format, (const char *)key, format_real);
    if (0 != err)
    {
        return err;
    }


    if (0 == strcmp(output, ""))
    {
        strcpy(output, DEFAULT_OUTPUT);
    }
    err = add_output(category_output, (const char *)key, output);

    return err;
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
    T_ASSERT(NULL != category_level);
    T_ASSERT(NULL != category_format);
    T_ASSERT(NULL != category_output);
    T_ASSERT(NULL != format_group);

    tint err = 0;
    /* check group exists first */
    if (t_keyfile_contains_group(keyfile, GROUP_NAME_RULES))
    {
        err = t_keyfile_group_foreach(keyfile, GROUP_NAME_RULES, rules_process);
    }
    else
    {
        err = add_level(category_level, DEFAULT_CATEGORY_NAME, DEFAULT_LEVEL);
        if (0 != err)
        {
            return err;
        }
        
        const tchar *format = get_format(format_group, DEFAULT_FORMAT_NAME);
        if (NULL == format)
        {
            format = DEFAULT_FORMAT;
        }
        err = add_format(category_format, DEFAULT_CATEGORY_NAME, format);
        if (0 != err)
        {
            return err;
        }
        err = add_output(category_output, DEFAULT_CATEGORY_NAME, DEFAULT_OUTPUT);
        if (0 != err)
        {
            return err;
        }
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

    if ((NULL == category_level) &&
        (NULL == category_format) &&
        (NULL == category_output) &&
        (NULL == format_group))
    {
        category_level = t_hash_string_new();
        if (NULL == category_level)
        {
            return -ENOMEM;
        }
        category_format = t_hash_string_new();
        if (NULL == category_format)
        {
            return -ENOMEM;
        }
        category_output = t_hash_string_new();
        if (NULL == category_output)
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
    }
    else
    {
        return -EEXIST;
    }

    return 0;
}

#if 1
static tint level_print(void *data)
{
    level_node *level = t_hash_string_entry((thash_string_node *)data, level_node, node);
    printf("category = %s, level = 0x%x\n", level->node.key, level->level);
    return 0;
}

static tint format_print(void *data)
{
    format_node *format = t_hash_string_entry((thash_string_node *)data, format_node, node);
    printf("category = %s, format = %s\n", format->node.key, format->value);
    return 0;
}

static tint output_print(void *data)
{
    output_node *output = t_hash_string_entry((thash_string_node *)data, output_node, node);
    printf("category = %s, output = %s\n", output->node.key, output->value);
    return 0;
}

static void print_level(void)
{
    t_hash_string_foreach(category_level, level_print);
}

static void print_format(void)
{
    t_hash_string_foreach(category_format, format_print);
}

static void print_output(void)
{
    t_hash_string_foreach(category_output, output_print);
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
    
    print_level();
    print_format();
    print_output();
    return 0;
}
