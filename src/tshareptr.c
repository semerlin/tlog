#include "tshareptr.h"
#include "tassert.h"
#include <stdlib.h>


/**
 * @brief init shared pointer struct
 */
void t_shareptr_init(tshareptr *ptr, tfree_func free_func, void *userdata)
{
    T_ASSERT(NULL != ptr);
    ptr->ref_count = 1;
    ptr->free_func = free_func;
    ptr->userdata = userdata;
}

/**
 * @brief add pointer reference count
 * @param ptr - data pointer
 * @return data pointer
 */
tshareptr *t_shareptr_ref(tshareptr *ptr)
{
    T_ASSERT(NULL != ptr);

    ptr->ref_count ++;
    return ptr;
}

/**
 * @brief unreference pointer count, if reachs zero then free pointer
 * @param ptr - data pointer
 */
void t_shareptr_unref(tshareptr **ptr)
{
    T_ASSERT(NULL != ptr);
    if ((*ptr)->ref_count <= 0)
    {
        return ;
    }

    (*ptr)->ref_count --;
    if (0 >= (*ptr)->ref_count)
    {
        if (NULL != (*ptr)->free_func)
        {
            (*ptr)->free_func((*ptr)->userdata);
            *ptr = NULL;
        }
    }
}


