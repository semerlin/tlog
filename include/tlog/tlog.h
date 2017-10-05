#ifndef _TLOG_H_
#define _TLOG_H_

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum 
{
    TLOG_DEBUG = 0x01,
    TLOG_INFO = 0x02,
    TLOG_WARN = 0x04,
    TLOG_ERROR = 0x08,
    TLOG_FATAL = 0x10
}tlog_level;


typedef struct _tlog_category tlog_category;

extern int tlog_init(const char *cfg_file);
extern const tlog_category *tlog_get_category(const char *name);


#ifdef  __cplusplus
}
#endif


#endif /* _TLOG_H_ */
