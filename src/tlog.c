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

/****************************************************
 * static variable 
 ****************************************************/
/* key = format name, value = format */
static thash_string *formats_kv = NULL;
/* key = category name, value = category_node */
static thash_string *category_detail = NULL;

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
    if (NULL != category_detail)
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
    category_free(category_detail);
    format_free(formats_kv);
    category_detail = NULL;
    formats_kv = NULL;
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


