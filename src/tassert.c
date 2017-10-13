/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <stdio.h>
#include "tassert.h"
#include "tsysdeps.h"

/****************************************************
 * macros definition
 ****************************************************/

/****************************************************
 * struct definition
 ****************************************************/

/****************************************************
 * static variable 
 ****************************************************/

/****************************************************
 * functions 
 ****************************************************/
#ifdef T_ENABLE_ASSERT
/**
 * @brief assert function
 * @param condition - assert condition result
 * @param condition_text - assert condition text
 * @param file - file of the assert
 * @param line - line of the assert
 * @param func - function of the assert
 */
void _tlog_assert(tbool condition, const tchar *condition_text, 
                  const tchar *file, tint line, const tchar *func)
{
    if (T_UNLIKELY(!condition))
    {
        fprintf(stderr, "assertion failed \"%s\" [file: %s] [line: %d] [function: %s]\n",
                condition_text, file, line, func);
        t_abort();
    }
}
#endif
