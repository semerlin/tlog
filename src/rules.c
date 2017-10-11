#include <string.h>
#include "tkeyfile.h"
#include "thash_string.h"
#include "rules.h"
#include "tassert.h"
#include "format.h"
#include "category.h"
#include "global.h"
#include "tstring.h"


/****************************************************
 * struct definition
 ****************************************************/
/* rules foreach userdata */
typedef struct 
{
    thash_string *cat_hash;
    const thash_string *format_hash;
}rule_userdata;


/****************************************************
 * functions 
 ****************************************************/

/**
 * @brief split rules string to level
 * @param name - category name string to split
 * @param rules - rules string to split
 * @param level - level string output
 */
static void split_category_and_level(const tchar *rules, tchar *name, tchar *level)
{
    T_ASSERT(NULL != rules);
    T_ASSERT(NULL != level);

    tint index = t_string_find_char(rules, 0, '.', TRUE);
    tchar buf[256];
    if (-1 != index)
    {
        t_string_left(rules, index, buf);
        t_string_trimmed(buf, name);
        t_string_right(rules, strlen(rules) - index - 1, buf);
        t_string_trimmed(buf, level);
    }
    else
    {
        t_string_trimmed(rules, name);
        level[0] = '*';
        level[1] = '\0';
    }
}

/**
 * @brief split rules string to format and output
 * @param rules - rules string to split
 * @param format - format string output
 * @param output - ouput string output
 */
static void split_format_and_output(const tchar *rules, tchar *format, tchar *output)
{
    T_ASSERT(NULL != rules);
    T_ASSERT(NULL != format);
    T_ASSERT(NULL != output);

    tint index = t_string_find_char(rules, 0, ';', TRUE);
    tchar buf[256];
    if (-1 != index)
    {
        t_string_left(rules, index, buf);
        t_string_trimmed(buf, format);
        t_string_right(rules, strlen(rules) - index - 1, buf);
        t_string_trimmed(buf, output);
    }
    else
    {
        t_string_trimmed(rules, format);
        output[0] = '\0';
    }
}

/**
 * @brief process rules and insert to hash table
 * @param key - category name
 * @param value - rules
 * @param userdata - user data
 * @return error code, 0 means no error
 */
static tint rules_process(void *key, void *value, void *userdata)
{
    T_ASSERT(NULL != userdata);
    tchar category[256];
    tchar level[256];
    tchar format[256];
    tchar output[256];
    rule_userdata *data = (rule_userdata *)userdata;
    split_category_and_level((const tchar *)key, category, level);
    split_format_and_output((const tchar *)value, format, output);
    return add_category(data->cat_hash, data->format_hash, category, level, format, output);
}


/**
 * @brief filter rules in configure file
 *        rule example: category_name.level=output;format
 * @param keyfile - configure file handle
 * @return 0 means no error
 */
tint filter_rules(const tkeyfile *keyfile, const thash_string *format_hash, thash_string **cat_hash)
{ 
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != format_hash);
    T_ASSERT(NULL != cat_hash);

    tint err = 0;
    err = categories_init(keyfile, cat_hash);
    if (0 != err)
    {
        return err;
    }

    /* check rules group key-value count */
    if (0 == t_keyfile_key_count(keyfile, GROUP_NAME_RULES))
    {
        const tchar *format = get_format(format_hash, DEFAULT_FORMAT_NAME);
        if (NULL == format)
        {
            format = DEFAULT_FORMAT;
        }
 
        err = add_category(*cat_hash, format_hash, DEFAULT_CATEGORY_NAME, DEFAULT_LEVEL,
                format, DEFAULT_OUTPUT);
    }
    else
    {
        rule_userdata data = {*cat_hash, format_hash};
        err = t_keyfile_group_foreach(keyfile, GROUP_NAME_RULES, rules_process, &data);
    }

    return err;
}

