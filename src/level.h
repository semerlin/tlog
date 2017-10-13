/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "ttypes.h"

T_BEGIN_DECLS

/* level mask definition */
#define LEVEL_MASK         0xff00
#define LEVEL_INDEX_MASK   0x00ff

T_EXTERN tuint32 log_level_convert(const tchar *level);

T_END_DECLS

#endif /* _LEVEL_H_ */
