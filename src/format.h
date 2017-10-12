#ifndef _FORMAT_H_
#define _FORMAT_H_

#include "ttypes.h"
#include "tkeyfile.h"
#include "thash_string.h"

T_BEGIN_DECLS

typedef struct _split_format split_format;

/* preprocess information */
typedef struct
{
    const tchar *file;
    const tchar *func;
    tint line;
    const tchar *line_str;
    tuint32 level;
    const tchar *user_msg;
}preprocess_info;

T_EXTERN thash_string *format_new(void);
T_EXTERN void format_free(thash_string *format);
T_EXTERN tint filter_format(const tkeyfile *keyfile, thash_string **hash);
T_EXTERN const tchar *get_format(const thash_string *hash, const tchar *name);
T_EXTERN const split_format *get_format_split(const thash_string *hash, const tchar *name);
T_EXTERN tbool format_validation(const tchar *format, tuint32 *count);
T_EXTERN split_format *format_to_split(const tchar *format);
T_EXTERN tuint32 format_split_to_string(tchar *buf, const split_format *splits, const preprocess_info *pre);
T_EXTERN void split_format_free(split_format *split);

T_END_DECLS

#endif /* _FORMAT_H_ */
