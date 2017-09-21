#ifndef _TSTRING_H_
#define _TSTRING_H_

#include "ttypes.h"

T_BEGIN_DECLS



T_EXTERN void t_string_left(const tchar *str, tuint32 len, tchar *left);
T_EXTERN void t_string_right(const tchar *str, tuint32 len, tchar *right);
T_EXTERN void t_string_mid(const tchar *str, tuint32 index, tuint32 len, tchar *mid);
T_EXTERN tbool t_string_contain_char(const tchar *str, tchar check, tbool cs);
T_EXTERN tint t_string_find_char(const tchar *str, tuint32 index, tchar check, tbool cs);
T_EXTERN tint t_string_find_char_reverse(const tchar *str, tuint32 index, tchar check, tbool cs);
T_EXTERN tbool t_string_contain_string(const tchar *str, const tchar *check, tbool cs);
T_EXTERN tint t_string_find_string(const tchar *str, tuint32 index, const tchar *check, tbool cs);
T_EXTERN tint t_string_find_string_reverse(const tchar *str, tuint32 index, const tchar *check, tbool cs);
T_EXTERN void t_string_trimmed(const tchar *str, tchar *out);
T_EXTERN void t_string_trimmed_head(const tchar *str, tchar *out);
T_EXTERN void t_string_trimmed_tail(const tchar *str, tchar *out);



T_END_DECLS


#endif /* _TSTRING_H_ */
