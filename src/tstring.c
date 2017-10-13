/**
 * This file is part of the tlog Library.
 *
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <string.h>
#include <ctype.h>
#include "tstring.h"
#include "tassert.h"

/****************************************************
 * macros definition
 ****************************************************/

/****************************************************
 * struct definition
 ****************************************************/

/****************************************************
 * static variable 
 ****************************************************/

/****************************************************
 * functions 
 ****************************************************/
/**
 * @brief get string left data
 * @param str - source string
 * @param len - left length
 * @param left - output string
 */
void t_string_left(const tchar *str, tuint32 len, tchar *left)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != left);

    tint str_len = strlen(str);
    str_len = (str_len > len) ? len : str_len;
    strncpy(left, str, str_len);
    left[str_len] = '\0';
}

/**
 * @brief get string right data
 * @param str - source string
 * @param len - right length
 * @param right - output string
 */
void t_string_right(const tchar *str, tuint32 len, tchar *right)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != right);

    tint str_len = strlen(str);
    tint index = ((str_len < len) ? 0 : (str_len - len));
    strncpy(right, str + index, len);
    right[len] = '\0';
}

/**
 * @brief get string middle data
 * @param str - source string
 * @param index - start position
 * @param len - middle string length
 * @param mid - output string
 */
void t_string_mid(const tchar *str, tuint32 index, tuint32 len, tchar *mid)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != mid);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index <= str_len);
#endif

    t_string_left(str + index, len, mid);
}

/**
 * @brief check if string contain given character
 * @param str - source string
 * @param check - given character
 * @param cs - TRUE: case sensitive
 *             FALSE: ignore case of characters
 */
tbool t_string_contain_char(const tchar *str, tchar check, tbool cs)
{
    T_ASSERT(NULL != str);

    tchar cur = 0;
    tbool ret = FALSE;
    while(0 != (cur = *str++))
    {
        cur = (cs ? cur : tolower(cur));
        check = (cs ? check : tolower(check));
        if (cur == check)
        {
            ret = TRUE;
            break;
        }
    }

    return ret;
}

/**
 * @brief find given character forward from string
 * @param str - source string
 * @param index - start find position
 * @param check - given character
 * @param cs - TRUE: case sensitive
 *             FALSE: ignore case of characters
 * @return given character position base of 0, -1 means can not find
 */
tint t_string_find_char(const tchar *str, tuint32 index, tchar check, tbool cs)
{
    T_ASSERT(NULL != str);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index <= str_len);
#endif

    str += index;
    tchar cur = 0;
    tint ret = -1;
    const tchar *origin = str;
    while(0 != (cur = *str))
    {
        cur = (cs ? cur : tolower(cur));
        check = (cs ? check : tolower(check));
        if (cur == check)
        {
            ret = str - origin;
            break;
        }
        str++;
    }

    return ret;
}

/**
 * @brief find given character backward from string
 * @param str - source string
 * @param index - start find position
 * @param check - given character
 * @param cs - TRUE: case sensitive
 *             FALSE: ignore case of characters
 * @return given character position base of 0, -1 means can not find
 */
tint t_string_find_char_reverse(const tchar *str, tuint32 index, tchar check, tbool cs)
{
    T_ASSERT(NULL != str);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index <= str_len);
#endif

    str += index;
    tchar cur = 0;
    tint ret = -1;
    const tchar *origin = str;
    while(0 != (cur = *str))
    {
        cur = (cs ? cur : tolower(cur));
        check = (cs ? check : tolower(check));
        if (cur == check)
        {
            ret = str - origin;
        }
        str++;
    }

    return ret;
}

/**
 * @brief find given character repeat times in string
 * @param str - source string
 * @param index - start find position
 * @param check - given character
 * @param cs - TRUE: case sensitive
 *             FALSE: ignore case of characters
 * @return character repeat times
 */
tuint32 t_string_char_count(const tchar *str, tuint32 index, tchar check, tbool cs)
{
    T_ASSERT(NULL != str);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index <= str_len);
#endif

    str += index;
    tchar cur = 0;
    tuint32 count = 0;
    while(0 != (cur = *str))
    {
        cur = (cs ? cur : tolower(cur));
        check = (cs ? check : tolower(check));
        if (cur == check)
        {
            count ++;
        }
        str++;
    }

    return count;
}

/**
 * @brief check if string contain given string
 * @param str - source string
 * @param check - given string
 * @param cs - TRUE: case sensitive
 *             FALSE: ignore case of characters
 */
tbool t_string_contain_string(const tchar *str, const tchar *check, tbool cs)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != check);

    tint index = -1;
    tint check_len = strlen(check);
    tbool ret = FALSE;
    if (check_len > strlen(str))
    {
        return FALSE;
    }
    else
    {
        while(1)
        {
            index = t_string_find_char(str, 0, *check, cs);
            if (-1 != index)
            {
                if (!cs)
                {
                    if (0 == strncasecmp(str + index, check, check_len))
                    {
                        ret = TRUE;
                        break;
                    }
                }
                else
                {
                    if (0 == strncmp(str + index, check, check_len))
                    {
                        ret = TRUE;
                        break;
                    }

                }

                str += (index + 1);
            }
            else
            {
                break;
            }
        }
    }

    return ret;
}

/**
 * @brief find given string forward from string
 * @param str - source string
 * @param index - start find position
 * @param check - given string
 * @param cs - TRUE: case sensitive
 *             FALSE: ignore case of characters
 * @return given string position base of 0, -1 means can not find
 */
tint t_string_find_string(const tchar *str, tuint32 index, const tchar *check, tbool cs)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != check);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index <= str_len);
#endif

    tint char_index = 0;
    tint check_len = strlen(check);
    tint ret = -1;
    if (check_len > strlen(str))
    {
        return -1;
    }
    else
    {
        str += index;
        const tchar *origin = str;
        while(-1 != char_index)
        {
            char_index = t_string_find_char(str, 0, *check, cs);
            if (-1 != char_index)
            {
                if (!cs)
                {
                    if (0 == strncasecmp(str + char_index, check, check_len))
                    {
                        ret = str + char_index - origin;
                        break;
                    }
                }
                else
                {
                    if (0 == strncmp(str + char_index, check, check_len))
                    {
                        ret = str + char_index - origin;
                        break;
                    }

                }

                str += (char_index + 1);
            }
        }
    }

    return ret;
}

/**
 * @brief find given string backward from string
 * @param str - source string
 * @param index - start find position
 * @param check - given string
 * @param cs - TRUE: case sensitive
 *             FALSE: ignore case of characters
 * @return given string position base of 0, -1 means can not find
 */
tint t_string_find_string_reverse(const tchar *str, tuint32 index, const tchar *check, tbool cs)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != check);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index <= str_len);
#endif

    tint char_index = 0;
    tint check_len = strlen(check);
    tint ret = -1;
    if (check_len > strlen(str))
    {
        return -1;
    }
    else
    {
        str += index;
        const tchar *origin = str;
        while(-1 != char_index)
        {
            char_index = t_string_find_char(str, 0, *check, cs);
            if (-1 != char_index)
            {
                if (!cs)
                {
                    if (0 == strncasecmp(str + char_index, check, check_len))
                    {
                        ret = str + char_index - origin;
                    }
                }
                else
                {
                    if (0 == strncmp(str + char_index, check, check_len))
                    {
                        ret = str + char_index - origin;
                    }

                }

                str += (char_index + 1);
            }
        }
    }

    return ret;
}

/**
 * @brief trimmed space in string head and tail
 * @param str - string to trim
 * @param out - trimmed string
 */
void t_string_trimmed(const tchar *str, tchar *out)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != out);
    while ('\0' != *str)
    {
        if (' ' == *str)
        {
            str++;
        }
        else
        {
            break;
        }
    }
    tint len = strlen(str);
    tbool has_return = FALSE;
    if ('\n' == str[len - 1])
    {
        len--;
        has_return = TRUE;
    }
    while(0 != len)
    {
        if ((' ' == str[len - 1]) || ('\t' == str[len - 1]))
        {
            len--;
        }
        else
        {
            break;
        }
    }
    strncpy(out, str, len);
    if (has_return)
    {
        out[len] = '\n';
        len++;
    }
    out[len] = '\0';
}


/**
 * @brief trimmed space in string head
 * @param str - string to trim
 * @param out - trimmed string
 */
void t_string_trimmed_head(const tchar *str, tchar *out)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != out);
    while ('\0' != *str)
    {
        if (' ' == *str)
        {
            str++;
        }
        else
        {
            break;
        }
    }
    strcpy(out, str);
}

/**
 * @brief trimmed space in string tail
 * @param str - string to trim
 * @param out - trimmed string
 */
void t_string_trimmed_tail(const tchar *str, tchar *out)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != out);
    tint len = strlen(str);
    while(0 != len)
    {
        if (' ' == str[len - 1])
        {
            len--;
        }
        else
        {
            break;
        }
    }
    strncpy(out, str, len);
    out[len] = '\0';
}


/**
 * @brief convert string to int
 * @param str - string to convert
 * @param out - output int
 * @return TRUE: success FALSE: failed
 */
tbool t_string_to_int(const tchar *str, tint *out)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != out);

    *out = 0;
    if ('\0' == *str)
    {
        return FALSE;
    }

    if (('-' == *str) && ('\0' == str[1]))
    {
        return FALSE;
    }

    tint sign = 1;
    if ('-' == *str)
    {
        sign = -1;
        str++;
    }

    while ('\0' != *str)
    {
        if((*str >= '0') && (*str <= '9'))
        {
            *out *= 10;
            *out += *str - 0x30;
            str++;
        }
        else
        {
            return FALSE;
        }
    }

    *out *= sign;

    return TRUE;
}

/**
 * @brief convert string to bool 
 * @param str - string to convert
 * @param out - output bool
 * @return TRUE: success FALSE: failed
 */
tbool t_string_to_bool(const tchar *str, tbool *out)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != out);

    if (0 == strcasecmp(str, "false"))
    {
        *out = FALSE;
    }
    else if (0 == strcasecmp(str, "true"))
    {
        *out = TRUE;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief remove line breaks in string
 * @param str - string to remove line break
 * @param out - string removed line break
 */
void t_string_remove_linebreak(const tchar *str, tchar *out)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != out);

    tint len = strlen(str);
    while (0 != len)
    {
        if (('\n' == str[len - 1]) || ('\r' == str[len - 1]))
        {
            len--;
        }
        else
        {
            break;
        }
    }

    strncpy(out, str, len);
    out[len] = '\0';
}

/**
 * @brief get one line from buf
 * @param out - output buffer
 * @param buf - parse string
 * @param index - parse start index
 */
tint32 t_string_get_line(tchar *out, const tchar *buf, tuint32 index)
{
    T_ASSERT(NULL != buf);
    T_ASSERT(index <= strlen(buf));
    buf += index;

    if ('\0' == *buf)
    {
        return -1;
    }

    T_ASSERT(NULL != out);
    const tchar *head = out;
    while(('\0' != *buf) &&
          ('\n' != *buf))
    {
        *out++ = *buf++;
    }
    *out = '\0';
    return out - head;
}

