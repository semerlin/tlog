#ifndef _TKEYFILE_H_
#define _TKEYFILE_H_

#include "ttypes.h"

T_BEGIN_DECLS


typedef struct _tkeyfile tkeyfile;

T_EXTERN tkeyfile *t_keyfile_new(void);
T_EXTERN void t_keyfile_free(tkeyfile *file);
T_EXTERN tint t_keyfile_load_from_file(tkeyfile *keyfile, const tchar *file);
T_EXTERN void t_keyfile_get_string(const tkeyfile *keyfile, const tchar *group, 
        const tchar *key, tchar *out, const tchar *default_value);
T_EXTERN tint t_keyfile_get_int(const tkeyfile *keyfile, const tchar *group, 
        const tchar *key, const tint default_value);
T_EXTERN tbool t_keyfile_get_bool(const tkeyfile *keyfile, const tchar *group, 
        const tchar *key, const tbool default_value);
T_EXTERN tbool t_keyfile_has_group(tkeyfile *keyfile, const tchar *group);
T_EXTERN tbool t_keyfile_has_key(tkeyfile *keyfile, const tchar *group, const tchar *key);
T_EXTERN tkeyfile *t_keyfile_ref(tkeyfile *keyfile);
T_EXTERN void t_keyfile_unref(tkeyfile **keyfile);


T_END_DECLS

#endif /* _TKEYFILE_H_ */
