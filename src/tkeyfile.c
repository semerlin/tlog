#include "tkeyfile.h"
#include "tassert.h"
#include "tshareptr.h"


struct _tkeyfile
{
    tshareptr ptr;
};



tkeyfile *t_keyfile_new()
{
}


void t_Keyfile_free(tkeyfile *file)
{
}

tint t_keyfile_load_from_file(tkeyfile *key_file, const tchar *file)
{
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


