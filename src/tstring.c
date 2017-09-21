#include "tstring.h"
#include "tassert.h"
#include <string.h>
#include <ctype.h>


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
    strncpy(left, str, ((str_len > len) ? str_len : len));
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
    T_ASSERT(index < str_len);
#endif

    t_string_left(str + index, len, mid);
}

/**
 * @brief check if string contain given character
 * @param str - source string
 * @param check - given character
 * @param cs - TRUE: ignore case of characters
 *             FALSE: case sensitive
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
 * @param cs - TRUE: ignore case of characters
 *             FALSE: case sensitive
 * @return given character position base of 0, -1 means can not find
 */
tint t_string_find_char(const tchar *str, tuint32 index, tchar check, tbool cs)
{
    T_ASSERT(NULL != str);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index < str_len);
#endif

    str += index;
    tchar cur = 0;
    tint ret = -1;
    const tchar *origin = str;
    while(0 != (cur = *str))
    {
        cur = (cs ? tolower(cur) : cur);
        check = (cs ? tolower(check) : check);
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
 * @param cs - true: ignore case of characters
 *             false: case sensitive
 * @return given character position base of 0, -1 means can not find
 */
tint t_string_find_char_reverse(const tchar *str, tuint32 index, tchar check, tbool cs)
{
    T_ASSERT(NULL != str);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index < str_len);
#endif

    str += index;
    tchar cur = 0;
    tint ret = -1;
    const tchar *origin = str;
    while(0 != (cur = *str))
    {
        cur = (cs ? tolower(cur) : cur);
        check = (cs ? tolower(check) : check);
        if (cur == check)
        {
            ret = str - origin;
        }
        str++;
    }

    return ret;
}

/**
 * @brief check if string contain given string
 * @param str - source string
 * @param check - given string
 * @param cs - TRUE: ignore case of characters
 *             FALSE: case sensitive
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
 * @param cs - TRUE: ignore case of characters
 *             FALSE: case sensitive
 * @return given string position base of 0, -1 means can not find
 */
tint t_string_find_string(const tchar *str, tuint32 index, const tchar *check, tbool cs)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != check);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index < str_len);
#endif

    tint char_index = -1;
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
        while(1)
        {
            char_index = t_string_find_char(str, 0, *check, cs);
            if (-1 != char_index)
            {
                if (!cs)
                {
                    if (0 == strncasecmp(str + char_index, check, check_len))
                    {
                        ret = str - origin;
                        break;
                    }
                }
                else
                {
                    if (0 == strncmp(str + char_index, check, check_len))
                    {
                        ret = str - origin;
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
 * @param cs - TRUE: ignore case of characters
 *             FALSE: case sensitive
 * @return given string position base of 0, -1 means can not find
 */
tint t_string_find_string_reverse(const tchar *str, tuint32 index, const tchar *check, tbool cs)
{
    T_ASSERT(NULL != str);
    T_ASSERT(NULL != check);

#ifdef T_ENABLE_ASSERT
    tint str_len = strlen(str);
    T_ASSERT(index < str_len);
#endif

    tint char_index = -1;
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
        while(1)
        {
            char_index = t_string_find_char(str, 0, *check, cs);
            if (-1 != char_index)
            {
                if (!cs)
                {
                    if (0 == strncasecmp(str + char_index, check, check_len))
                    {
                        ret = str - origin;
                    }
                }
                else
                {
                    if (0 == strncmp(str + char_index, check, check_len))
                    {
                        ret = str - origin;
                    }

                }

                str += (char_index + 1);
            }
        }
    }

    return ret;
}


