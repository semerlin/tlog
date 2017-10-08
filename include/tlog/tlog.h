#ifndef _TLOG_H_
#define _TLOG_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum 
{
    TLOG_DEBUG = 0x01,
    TLOG_INFO = 0x02,
    TLOG_NOTICE = 0x04,
    TLOG_WARN = 0x08,
    TLOG_ERROR = 0x10,
    TLOG_FATAL = 0x20,
}tlog_level;


typedef struct _tlog_category tlog_category;

extern int tlog_init(const char *cfg_file);
extern const tlog_category *tlog_get_category(const char *name);
extern void tlog(const tlog_category *cat, const char *file,
        int line, const char *func, int level, const char *fmt, ...);

/* useful macros */
#define zlog_debug(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, TLOG_DEBUG, __VA_ARGS__)
#define zlog_info(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, TLOG_INFO, __VA_ARGS__)
#define zlog_notice(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, TLOG_NOTICE, __VA_ARGS__)
#define zlog_warn(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, TLOG_WARN, __VA_ARGS__)
#define zlog_error(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, TLOG_ERROR, __VA_ARGS__)
#define zlog_fatal(cat, ...) \
    tlog(cat, __FILE__, __LINE__, __func__, TLOG_FATAL, __VA_ARGS__)








#ifdef  __cplusplus
}
#endif


#endif /* _TLOG_H_ */
