/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef _TLOG_H_
#define _TLOG_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum 
{
    TLOG_DEBUG = 0x0100,
    TLOG_INFO = 0x0201,
    TLOG_NOTICE = 0x0402,
    TLOG_WARN = 0x0803,
    TLOG_ERROR = 0x1004,
    TLOG_FATAL = 0x2005,
}tlog_level;


typedef struct _tlog_category tlog_category;

extern int tlog_init_from_file(const char *file);
extern int tlog_init_from_data(const char *data);
extern const tlog_category *tlog_get_category(const char *name);
extern void tlog(const tlog_category *cat, const char *file,
        long line, const char *func, const char *line_str,
        int level, const char *fmt, ...);

#define _STR(s)   #s 
#define STR(s)    _STR(s)

/* function name */
#if defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    #define __func__ __FUNCTION__
#elif defined(__GNUC__)
    #define __func__ __PRETTY_FUNCTION__
#else
    #define __func__ "<unknown>"
#endif

#define tlog_debug(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, STR(__LINE__), TLOG_DEBUG, __VA_ARGS__)
#define tlog_info(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, STR(__LINE__), TLOG_INFO, __VA_ARGS__)
#define tlog_notice(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, STR(__LINE__), TLOG_NOTICE, __VA_ARGS__)
#define tlog_warn(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, STR(__LINE__), TLOG_WARN, __VA_ARGS__)
#define tlog_error(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, STR(__LINE__), TLOG_ERROR, __VA_ARGS__)
#define tlog_fatal(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, STR(__LINE__), TLOG_FATAL, __VA_ARGS__)


#ifdef  __cplusplus
}
#endif


#endif /* _TLOG_H_ */
