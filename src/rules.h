#ifndef _RULES_H_
#define _RULES_H_

#include "ttypes.h"

T_BEGIN_DECLS

tint filter_rules(const tkeyfile *keyfile, const thash_string *format_hash, thash_string **cat_hash);

T_END_DECLS

#endif /* _RULES_H_ */
