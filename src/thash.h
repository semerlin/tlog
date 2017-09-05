#ifndef _THASH_H_
#define _THASH_H_

#include "ttypes.h"
#include "tcommon.h"

T_BEGIN_DECLS

typedef struct _thash thash;
typedef struct _thash_node thash_node;

struct _thash_node
{
    thash_node *next;
};

T_EXTERN thash *t_hash_new(void);
T_EXTERN void t_hash_insert(thash *hash, const char *key, thash_node *node);
T_EXTERN thash_node *t_hash_find(thash *hash, const char *key);
T_EXTERN tbool t_hash_contain(thash *hash, const char *key);
T_EXTERN thash_node *t_hash_remove(thash *hash, const char *key);
T_EXTERN void t_hash_free(thash *hash, tfree_callback freefunc);


T_END_DECLS

#endif /* _THASH_H_ */
