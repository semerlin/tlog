/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef TTYPES_H_
#define TTYPES_H_

#include "tconfig.h"
#include "tmacros.h"

T_BEGIN_DECLS

/* common types */
typedef char   tchar;
typedef short  tshort;
typedef long   tlong;
typedef int    tint;
typedef tint   tbool;

typedef unsigned char   tuchar;
typedef unsigned short  tushort;
typedef unsigned long   tulong;
typedef unsigned int    tuint;

typedef float   tfloat;
typedef double  tdouble;

/* boolean definition */
#ifndef	FALSE
#define	FALSE	(0)
#endif

#ifndef	TRUE
#define	TRUE	(!FALSE)
#endif


#ifndef NULL
#ifdef __cplusplus
    #define NULL    (0L)
#else
    #define NULL    ((void*) 0)
#endif
#endif


/* callback function */
typedef void (*tfree_func)(void *data);

/* general callback function */
typedef tint (*tgeneral_func)(void *data);

/* hash table callback function */
typedef tint (*thash_func)(void *data, void *userdata);

/* key value callback fucntion */
typedef tint (*tkey_value_func)(void *key, void *value, void *userdata);


T_END_DECLS

#endif /* TTYPES_H_ */
