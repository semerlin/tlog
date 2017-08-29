#include "tassert.h"
#include "tsysdeps.h"


void _tlog_assert(tbool condition, const tchar *condition_text, 
                  const tchar *file, tint line, const tchar *func)
{
    fprintf(stderr, "assertion failed \"%s\" [file: %s] [line: %s] [function: %s]",
            condition_text, file, line, func);
    t_abort();
}
