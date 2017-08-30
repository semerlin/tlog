#ifndef TASSERT_H_
#define TASSERT_H_

#include "ttypes.h"

T_BEGIN_DECLS


#ifdef T_DISABLE_ASSERT
    #define T_ASSERT(condition) do { } while (0)
#else
    void _tlog_assert(tbool condition,
                      const char  *condition_text,
                      const char  *file,
                      int          line,
                      const char  *func);
    #define T_ASSERT(condition) \
        _tlog_assert((condition) != 0, #condition, __FILE__, __LINE__, __FUNC__)
#endif


T_END_DECLS

#endif /* TASSERT_H_ */