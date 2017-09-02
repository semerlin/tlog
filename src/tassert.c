#include <stdio.h>
#include "tassert.h"
#include "tsysdeps.h"



#ifdef T_ENABLE_ASSERT
void _tlog_assert(tbool condition, const tchar *condition_text, 
                  const tchar *file, tint line, const tchar *func)
{
    fprintf(stderr, "assertion failed \"%s\" [file: %s] [line: %d] [function: %s]\n",
            condition_text, file, line, func);
    t_abort();
}
#endif
