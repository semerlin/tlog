#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "format.h"
#include "tassert.h"
#include "thash_string.h"
#include "global.h"


/****************************************************
 * struct definition
 ****************************************************/
/* format node */
typedef struct
{
    tchar *value;
    thash_string_node node;
}format_node;


/****************************************************
 * static variable 
 ****************************************************/


/****************************************************
 * functions 
 ****************************************************/

/**
 * @brief new format hash table
 * @return format hash table pointer
 */
thash_string *format_new(void)
{
    return t_hash_string_new();
}

/**
 * @brief get format value from hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @return format value
 */
const tchar *get_format(const thash_string *hash, const tchar *name)
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
 * @brief insert key and value to format hash table
 * @param key - format name string
 * @param value - format string
 * @return error code, 0 means no error
 */
static tint format_process(void *key, void *value, void *userdata)
{
    T_ASSERT(NULL != key);
    T_ASSERT(NULL != value);
    T_ASSERT(NULL != userdata);
    return add_format((thash_string *)userdata, key, value);
}

/**
 * @brief free format hash table
 * @param data - format node
 */
static void free_format(void *data)
{
    T_ASSERT(NULL != data);
    thash_string_node *string_node = (thash_string_node *)data;
    format_node *format = t_hash_string_entry(string_node, format_node, node);
    free(format->value);
    free(format->node.key);
    free(format);
}

/**
 * @brief filter output format group
 * @param keyfile - keyfile handle
 * @param hash - formats hash table
 * @return error code, 0 means no error
 */
tint filter_format(const tkeyfile *keyfile, thash_string *hash)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != hash);

    tint err = 0;


    if (0 != t_hash_string_count(hash))
    {
        t_hash_string_empty(hash, free_format);
    }

    /* check group exists first */
    if (t_keyfile_contains_group(keyfile, GROUP_NAME_FORMAT))
    {
        err = t_keyfile_group_foreach(keyfile, GROUP_NAME_FORMAT, format_process, hash);
    }
    else
    {
        err = add_format(hash, DEFAULT_FORMAT_NAME, DEFAULT_FORMAT);
    }

    if (0 == err)
    {
        /* add default format */
        if (!t_hash_string_contain(hash, DEFAULT_FORMAT_NAME))
        {
            err = add_format(hash, DEFAULT_FORMAT_NAME, DEFAULT_FORMAT);
        }
    }

    return err;
}

