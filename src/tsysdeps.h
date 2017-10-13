/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef _TSYSDEPS_H_
#define _TSYSDEPS_H_

#include "ttypes.h"

T_BEGIN_DECLS


T_EXTERN void t_exit(tint code);
T_EXTERN void t_print_backtrace(void);
T_EXTERN void t_abort(void);

T_END_DECLS

#endif /* _TSYSDEPS_H_ */
