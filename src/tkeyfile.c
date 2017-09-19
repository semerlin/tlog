#include "tkeyfile.h"
#include "tassert.h"
#include "tshareptr.h"
#include "thash_string.h"
#include "tlist.h"
#include <errno.h>

//key-value node
typedef struct
{
    char *value;
    thash_string_node node;
}

//group node
typedef struct
{
    thash_string *child;
    char *group_name;
    tlist node;
}group_node;

//keyfile structure
struct _tkeyfile
{
    tshareptr ptr;
    tlist groups;
};



tkeyfile *t_keyfile_new()
{
    tkeyfile *key_file = (tkeyfile *)malloc(sizeof(tkeyfile));
    t_shareptr_init((tshareptr *)key_file);
    return key_file;
}


void t_Keyfile_free(tkeyfile *file)
{
}

static tbool parse_group(const char *data, char *group)
{
    return TRUE;
}

tint t_keyfile_load_from_file(tkeyfile *key_file, const tchar *file)
{
    T_ASSERT(NULL != key_file);
    char buf[1024];
    char *line_str = NULL;

    if (NULL == file)
    {
    }

    FILE *fp = fopen(file, "r");
    if (NULL == fp)
    {
        return errno;
    }

    tbool start_group = FALSE;
    do
    {
        line_str = fgets(buf, 1024, fp);

    }while(NULL != line_str);
}

tint t_keyfile_load_from_data(tkeyfile *key_file, const tchar *data)
{
}

tchar *t_keyfile_get_string(const tkeyfile *key_file, const char *group, 
        const char *key, const char *default_value)
{
}

tint t_keyfile_get_int(const tkeyfile *key_file, const char *group, 
        const char *key, const tint default_value)
{
}

tbool t_keyfile_get_bool(const tkeyfile *key_file, const char *group, 
        const char *key, const tbool default_value)
{
}

tbool t_keyfile_has_group(tkeyfile *key_file, const tchar *group)
{
}

tbool t_keyfile_has_key(tkeyfile *key_file, const tchar *key)
{
}

tkeyfile *t_keyfile_ref(tkeyfile *key_file)
{
    T_ASSERT(NULL != key_file);
    return (tkeyfile *)t_shareptr_ref((t_shareptr *)key_file);
}

void *t_keyfile_unref(tkeyfile *key_file)
{
    T_ASSERT(NULL != keyfile);
    t_shareptr_unref((t_shareptr *)key_file);
}


