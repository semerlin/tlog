/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef _CATEGORY_H_
#define _CATEGORY_H_

#include "ttypes.h"
#include "../include/tlog/tlog.h"
#include <stdarg.h>

T_BEGIN_DECLS

T_EXTERN tint categories_init(const tkeyfile *keyfile, thash_string **hash);
T_EXTERN void category_free(thash_string *cat_hash);
T_EXTERN tint add_category(thash_string *cat_hash, const thash_string *format_hash, 
        const tchar *name, const tchar *level, 
        const tchar *format, const tchar *output);
T_EXTERN tlog_category *get_category(const thash_string *hash, 
        const tchar *name);
T_EXTERN void category_gen_log(const tlog_category *cat, const tchar *file, 
        tlong line, const tchar *func, const tchar *line_str,
        tuint32 level, const tchar *msg);

T_EXTERN void print_category(const thash_string *hash);

T_END_DECLS


#endif /* _CATEGORY_H_ */
