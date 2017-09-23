#ifndef _TSHAREPTR_H_
#define _TSHAREPTR_H_

#include "ttypes.h"

T_BEGIN_DECLS

typedef struct
{
    tint32 ref_count;
    tfree_callback free_func;
    void *userdata;
}tshareptr;

T_EXTERN void t_shareptr_init(tshareptr *ptr, tfree_callback free_func, void *userdata);
T_EXTERN tshareptr *t_shareptr_ref(tshareptr *ptr);
T_EXTERN void t_shareptr_unref(tshareptr **ptr);

T_END_DECLS




#endif
