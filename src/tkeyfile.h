#ifndef _TKEYFILE_H_
#define _TKEYFILE_H_

#include "ttypes.h"

T_BEGIN_DECLS


typedef _tkeyfile tkeyfile;

T_EXTERN tkeyfile *t_keyfile_new();
T_EXTERN void t_Keyfile_free(tkeyfile *file);
T_EXTERN tint t_keyfile_load_from_file(tkeyfile *key_file, const tchar *file);
T_EXTERN tint t_keyfile_load_from_data(tkeyfile *key_file, const tchar *data);
T_EXTERN tchar *t_keyfile_get_string(const tkeyfile *key_file, const char *group, 
        const char *key, const char *default_value);
T_EXTERN tint t_keyfile_get_int(const tkeyfile *key_file, const char *group, 
        const char *key, const tint default_value);
T_EXTERN tbool t_keyfile_get_bool(const tkeyfile *key_file, const char *group, 
        const char *key, const tbool default_value);
T_EXTERN tbool t_keyfile_has_group(tkeyfile *key_file, const tchar *group);
T_EXTERN tbool t_keyfile_has_key(tkeyfile *key_file, const tchar *key);
T_EXTERN tkeyfile *t_keyfile_ref(tkeyfile *key_file);
T_EXTERN void *t_keyfile_unref(tkeyfile *keyfile);

T_END_DECLS

#endif /* _TKEYFILE_H_ */
