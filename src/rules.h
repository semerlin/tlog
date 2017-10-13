/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef _RULES_H_
#define _RULES_H_

#include "ttypes.h"

T_BEGIN_DECLS

tint filter_rules(const tkeyfile *keyfile, const thash_string *format_hash, thash_string **cat_hash);

T_END_DECLS

#endif /* _RULES_H_ */
