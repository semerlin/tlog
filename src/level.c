#include <string.h>
#include "level.h"
#include "../include/tlog/tlog.h"
#include "tassert.h"




/****************************************************
 * struct definition
 ****************************************************/
/* level map */
struct
{
    const tchar name[16];
    const tlog_level level;
}log_level_info[] = 
{
    {"debug", TLOG_DEBUG},
    {"info", TLOG_INFO},
    {"notice", TLOG_NOTICE},
    {"warn", TLOG_WARN},
    {"error", TLOG_ERROR},
    {"fatal", TLOG_FATAL},
    {"", TLOG_DEBUG | TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL}
};


/**
 * @brief convert level string to level int value
 * @param level - level string
 * @return level int value
 */
tuint32 log_level_convert(const tchar *level)
{
    T_ASSERT(NULL != level);

    const tchar *plevel = level;

    /* get level flag */
    tuint8 level_flag = 0;
    tuint8 start_index = 0;
    tuint8 info_count = sizeof(log_level_info) / sizeof(log_level_info[0]);

    if ('>' == *plevel)
    {
        plevel++;
        level_flag = 2;
        start_index = 1;
        if ('=' == *plevel)
        {
            level_flag++;
            start_index++;
        }
    }
    else if('=' == *plevel)
    {
        level_flag = 1;
        start_index = 1;
    }
    else if(('*' == plevel[0]) && ('\0' == plevel[1]))
    {
        return log_level_info[info_count - 1].level;
    }
    else if('\0' != *plevel)
    {
        level_flag = 3;
        start_index = 0;
    }
    else
    {
        return 0;
    }

    tuint32 level_value = 0;
    /* get level text */
    level += start_index;

    for (tuint8 i = 0; i < info_count; ++i)
    {
        if (0 == strcmp(log_level_info[i].name, level))
        {
            switch(level_flag)
            {
            case 1:
                /* "=" */
                level_value = log_level_info[i].level;
                break;
            case 2:
                /* ">" */
                level_value = log_level_info[info_count - 1].level;
                level_value &= ~(((log_level_info[i].level - 1) << 1) + 1);
                break;
            case 3:
            default:
                /* ">=" */
                level_value = log_level_info[info_count - 1].level;
                level_value &= ~(log_level_info[i].level - 1);
                break;
            }
            break;
        }
    }

    return level_value;
}



