/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef _TKEYFILE_H_
#define _TKEYFILE_H_

#include "ttypes.h"

T_BEGIN_DECLS


typedef struct _tkeyfile tkeyfile;

T_EXTERN tkeyfile *t_keyfile_new(void);
T_EXTERN void t_keyfile_free(tkeyfile *file);
T_EXTERN void t_keyfile_use_last_sep(tkeyfile *file, tbool flag);
T_EXTERN tint t_keyfile_load_from_file(tkeyfile *keyfile, const tchar *file);
T_EXTERN tint t_keyfile_load_from_data(tkeyfile *keyfile, const tchar *buf);
T_EXTERN void t_keyfile_get_string(const tkeyfile *keyfile, const tchar *group, 
        const tchar *key, tchar *out, const tchar *default_value);
T_EXTERN tint t_keyfile_get_int(const tkeyfile *keyfile, const tchar *group, 
        const tchar *key, const tint default_value);
T_EXTERN tbool t_keyfile_get_bool(const tkeyfile *keyfile, const tchar *group, 
        const tchar *key, const tbool default_value);
T_EXTERN tint t_keyfile_group_foreach(const tkeyfile *keyfile, const tchar *group_name,
        tkey_value_func kv_func, void *userdata);
T_EXTERN void t_keyfile_group_names(const tkeyfile *keyfile, tchar **groups);
T_EXTERN void t_keyfile_key_names(const tkeyfile *keyfile, const tchar *group_name, tchar **keys);
T_EXTERN tuint32 t_keyfile_group_count(const tkeyfile *keyfile);
T_EXTERN tuint32 t_keyfile_key_count(const tkeyfile *keyfile, const tchar *group_name);
T_EXTERN tbool t_keyfile_contains_group(const tkeyfile *keyfile, const tchar *group);
T_EXTERN tbool t_keyfile_contains_key(const tkeyfile *keyfile, const tchar *group, const tchar *key);
T_EXTERN tkeyfile *t_keyfile_ref(tkeyfile *keyfile);
T_EXTERN void t_keyfile_unref(tkeyfile **keyfile);


T_END_DECLS

#endif /* _TKEYFILE_H_ */
