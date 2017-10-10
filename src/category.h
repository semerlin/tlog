#ifndef _CATEGORY_H_
#define _CATEGORY_H_

#include "ttypes.h"
#include "../include/tlog/tlog.h"

T_BEGIN_DECLS

T_EXTERN tint categories_init(const tkeyfile *keyfile, thash_string *hash);
T_EXTERN tint add_category(thash_string *cat_hash, const thash_string *format_hash, 
        const tchar *name, const tchar *level, 
        const tchar *format, const tchar *output);
T_EXTERN tlog_category *get_category(const thash_string *hash, 
        const tchar *name);

T_EXTERN void print_category(const thash_string *hash);

T_END_DECLS


#endif /* _CATEGORY_H_ */
