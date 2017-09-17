#ifndef _THASH_STRING_H_
#define _THASH_STRING_H_

#include "thlist.h"

T_BEGIN_DECLS

typedef struct _thash_string thash_string;

typedef struct 
{
    char *key;
    thlist_node node;
}thash_string_node;


T_EXTERN void t_hash_string_init_node(thash_string_node *node, const char *key);
T_EXTERN thash_string *t_hash_string_new(void);
T_EXTERN void t_hash_string_insert(thash_string *hash_string, thash_string_node *node);
T_EXTERN thash_string_node *t_hash_string_remove(thash_string *hash_string, const char *key);
T_EXTERN void t_hash_string_remove_node(thash_string_node *node);
T_EXTERN thash_string_node *t_hash_string_get(thash_string *hash_string, const char *key);
T_EXTERN tbool t_hash_string_contain(thash_string *hash_string, const char *key);
T_EXTERN void t_hash_string_free(thash_string *head, tfree_callback free_func);

T_END_DECLS

#endif /* _THASH_STRING_H_ */
