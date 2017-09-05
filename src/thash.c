#include "thash.h"

struct _thash_head
{
    thash_node *node;
};

struct _thash
{
    tuint32 table_size;
    tuint32 element_cout;
    struct _thash_head *head;
};


thash *t_hash_new(void)
{
}

void t_hash_insert(thash *hash, const char *key, thash_node *node)
{
}

thash_node *t_hash_find(thash *hash, const char *key)
{
}

tbool t_hash_contain(thash *hash, const char *key)
{
}

thash_node *t_hash_remove(thash *hash, const char *key)
{
}

t_hash_free(thash *hash, tfree_callback freefunc)
{
}




