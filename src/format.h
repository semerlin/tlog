#ifndef _FORMAT_H_
#define _FORMAT_H_

#include "ttypes.h"
#include "tkeyfile.h"
#include "thash_string.h"

T_BEGIN_DECLS

/****************************************************
 * macros definition
 ****************************************************/
#define GROUP_NAME_FORMAT        "format"
#define DEFAULT_FORMAT_NAME      "default"
#define DEFAULT_FORMAT           "[%t]"

T_EXTERN thash_string *format_new(void);
T_EXTERN tint filter_format(const tkeyfile *keyfile, thash_string *hash);
T_EXTERN const tchar *get_format(const thash_string *hash, const tchar *name);

T_END_DECLS

#endif /* _FORMAT_H_ */
