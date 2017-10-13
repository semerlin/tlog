/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef TMACROS_H_
#define TMACROS_H_

#include "tconfig.h"

/* we include stddef.h to get the system's definition of NULL */
#include <stddef.h>

/* 
 * if we have C99, or if we are using C++, then we can use "inline" directly.
 * Otherwise, we say "__inline" to avoid the warning.
 */
#ifndef __cplusplus
#if !defined(__STDC_VERSION__) || (__STDC_VERSION__ < 199900)
    #define T_INLINE_DEFINE_NEEDED
#endif
#endif

#ifdef T_INLINE_DEFINE_NEEDED
    #undef inline
    #define inline __inline
#endif

#undef T_INLINE_DEFINE_NEEDED

/* guard C code in headers, while including them from C++ */
#ifdef  __cplusplus
    #define T_BEGIN_DECLS  extern "C" {
    #define T_END_DECLS    }
#else
    #define T_BEGIN_DECLS
    #define T_END_DECLS
#endif

#undef	MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#undef	MIN
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

#undef	ABS
#define ABS(a)	   (((a) < 0) ? -(a) : (a))

#undef	CLAMP
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

/*
 * Count the number of elements in an array. 
 * The array can not be a dynamically allocated array 
 */
#define T_N_ELEMENTS(arr)		(sizeof(arr) / sizeof((arr)[0]))

/* handling structure */
#define T_OFFSET_OF(struct_type, member)    \
    ((unsigned long)&((struct_type *)0)->member)

/* get struct address */
#define T_CONTAINER_OF(member_ptr, struct_type, member)                     \
    ({                                                                      \
         const typeof(((struct_type *)0)->member) *__mptr = (member_ptr);   \
         (struct_type *)((char *)__mptr - T_OFFSET_OF(struct_type, member));       \
     })

/* likely and unlikely */
#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
    #define T_LIKELY(expr)   (__builtin_expect(!!(expr), 1))
    #define T_UNLIKELY(expr) (__builtin_expect(!!(expr), 0))
#else
    #define T_LIKELY(expr)   (expr)
    #define T_UNLIKELY(expr) (expr)
#endif

/* function name */
#if defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    #define __FUNC__ __func__
#elif defined(__GNUC__)
    #define __FUNC__ __PRETTY_FUNCTION__
#else
    #define __FUNC__ "unknown function"
#endif



/* export definition */
#ifndef T_EXTERN
#define T_EXTERN extern
#endif

#endif /* TMACROS_H_ */
