#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
 * static variable 
 ****************************************************/
/* key = format name, value = format */
static thash_string *formats_kv = NULL;
/* key = category name, value = category_node */
static thash_string *category_detail = NULL;


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
    tint err = 0;

    if (NULL == category_detail)
    {
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

        err = filter_format(keyfile, formats_kv);
        if (0 != err)
        {
            return err;
        }

        err = filter_rules(keyfile, formats_kv, category_detail);
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
    t_keyfile_use_last_sep(TRUE);
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
    
    print_category(category_detail);
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

    return get_category(category_detail, name);
}

void tlog(const tlog_category *cat, const char *file,
        int line, const char *func, int level, const char *fmt, ...)
{
}


