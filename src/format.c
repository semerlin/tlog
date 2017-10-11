#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "format.h"
#include "tassert.h"
#include "tstring.h"
#include "thash_string.h"
#include "global.h"


/****************************************************
 * struct definition
 ****************************************************/
/* split format */
typedef struct _split_format_single split_format_single;

typedef tuint32 (*splitformat_write)(tchar *buf, const split_format_single *split_single, const preprocess_info *pre);

struct _split_format_single
{
    tchar *data;
    /* 
     * -1: right alignment 
     *  1: left alignment
     */
    tint8 align;
    tuint8 width_min;
    /* -1: unlimit */
    tint8 width_max;
    splitformat_write write_buf;
};

struct _split_format
{
    tuint32 count;
    split_format_single *splits;
};

/* format node */
typedef struct
{
    tchar *value;
    split_format *splits;
    thash_string_node node;
}format_node;



/****************************************************
 * static variable 
 ****************************************************/
/*static tchar *upper_level[] = 
{
    "DEBUG",
    "INFO",
    "NOTICE",
    "WARN",
    "ERROR",
    "FATAL"
};

static tchar *lower_level[] = 
{
    "debug",
    "info",
    "notice",
    "warn",
    "error",
    "fatal"
};*/


/****************************************************
 * functions 
 ****************************************************/

static tuint32 write_direct(tchar *buf, const split_format_single *split_single,
        const preprocess_info *line)
{
    strcpy(buf, split_single->data);
    return strlen(split_single->data);
}

static tuint32 write_time(tchar *buf,  const split_format_single *split_single,
        const preprocess_info *line)
{
    return 0;
}

static tuint32 write_filename(tchar *buf,  const split_format_single *split_single,
        const preprocess_info *line)
{
    return 0;
}
static tuint32 write_filename_full(tchar *buf, const split_format_single *split_single,
        const preprocess_info *line)
{
    return 0;
}

static tuint32 write_line(tchar *buf, const split_format_single *split_single,
        const preprocess_info *line)
{
    return 0;
}

static tuint32 write_message(tchar *buf, const split_format_single *split_single,
        const preprocess_info *line)
{
    return 0;
}

static tuint32 write_level_lower(tchar *buf, const split_format_single *split_single,
        const preprocess_info *line)
{
    return 0;
}

static tuint32 write_level_upper(tchar *buf, const split_format_single *split_single,
        const preprocess_info *line)
{
    return 0;
}


/**
 * @brief new format hash table
 * @return format hash table pointer
 */
thash_string *format_new(void)
{
    return t_hash_string_new();
}

/**
 * @brief free format hash table
 * @param data - format node
 */
static void free_format(void *data)
{
    T_ASSERT(NULL != data);
    thash_string_node *string_node = (thash_string_node *)data;
    format_node *format = t_hash_string_entry(string_node, format_node, node);
    free(format->value);
    split_format_free(format->splits);
    free(format->node.key);
    free(format);
}

/**
 * @brief free format hash table
 * @param format - format hash table
 */
void format_free(thash_string *format)
{
    T_ASSERT(NULL != format);
    t_hash_string_empty(format, free_format);
}

/**
 * @brief get format value from hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @return format value
 */
const tchar *get_format(const thash_string *hash, const tchar *name)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);

    thash_string_node *string_node = t_hash_string_get(hash, name);
    if (NULL == string_node)
    {
        return NULL;
    }

    format_node *node = t_hash_string_entry(string_node, format_node, node);

    return node->value;
}

/**
 * @brief get split format from hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @return split format
 */
const split_format *get_format_split(const thash_string *hash, const tchar *name)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);

    thash_string_node *string_node = t_hash_string_get(hash, name);
    if (NULL == string_node)
    {
        return NULL;
    }

    format_node *node = t_hash_string_entry(string_node, format_node, node);

    return node->splits;
}


/**
 * @brief split format quickly
 * @param format - format string
 * @return split format pointer
 */
static split_format *format_to_split_quick(const tchar *format, tuint32 count)
{
    T_ASSERT(NULL != format);

    tuint32 split_count = count;

    split_format *splits = NULL;
    tint cur_index = 0, last_index = 0, char_index = -1;
    /* alloc split format space */
    splits = malloc(sizeof(split_format));
    if (NULL == splits)
    {
        return NULL;
    }

    splits->splits = calloc(sizeof(split_format_single), split_count);
    if (NULL == splits->splits)
    {
        free(splits);
        return NULL;
    }

    /* init split to default */
    for (tuint32 i = 0; i < split_count; ++i)
    {
        splits->splits[i].data = NULL;
        splits->splits[i].align = 1;
        splits->splits[i].width_min = 0;
        splits->splits[i].width_max = -1;
    }

    tint format_len = strlen(format);
    splits->count = split_count;
    split_count = 0;
    while(1)
    {
        tint align_flag = 1;
        char_index = t_string_find_char(format, last_index, '%', TRUE);
        if (-1 != char_index)
        {
            cur_index += char_index;
            if (0 != char_index)
            {
                /* add message string */
                splits->splits[split_count].data = malloc(cur_index - last_index + 1);
                if (NULL == splits->splits[split_count].data)
                {
                    goto ERROR;
                }
                strncpy(splits->splits[split_count].data, format + last_index, cur_index - last_index);
                splits->splits[split_count].data[cur_index - last_index] = '\0';
                splits->splits[split_count].write_buf = write_direct;
                split_count ++;
            }

            cur_index ++;
            last_index = cur_index;

            /* skip digital and % */
            if ('-' == format[cur_index])
            {
                align_flag = -1;
                cur_index ++;
                last_index = cur_index;
            }
            else if ('%' == format[cur_index])
            {
                /* add % */
                splits->splits[split_count].data = malloc(2);
                if (NULL == splits->splits[split_count].data)
                {
                    goto ERROR;
                }
                splits->splits[split_count].data[0] = '%';
                splits->splits[split_count].data[1] = '\0';
                splits->splits[split_count].write_buf = write_direct;
                split_count ++;
                cur_index ++;
                last_index = cur_index;
                continue;
            }

            while((format[cur_index] != '\0') && 
                  (('.' == format[cur_index]) ||
                  ((format[cur_index] >= '0') &&
                  (format[cur_index] <= '9'))))
            {
                cur_index ++;
            }

            /* get width value, max is 99 */
            tchar dig_min_buf[3] = "99";
            tchar dig_max_buf[3] = "99";
            tint dig_min = 0;
            tint dig_max = -1;
            tint dot_index = t_string_find_char(format + last_index, 0, '.', TRUE);
            if (-1 == dot_index)
            {
                if (cur_index - last_index <= 2)
                {
                    strncpy(dig_min_buf, format + last_index, cur_index - last_index);
                    dig_min_buf[cur_index - last_index] = '\0';
                }
                t_string_to_int(dig_min_buf, &dig_min);
            }
            else
            {
                if (dot_index <= 2)
                {
                    strncpy(dig_min_buf, format + last_index, dot_index);
                    dig_min_buf[dot_index] = '\0';
                }
                t_string_to_int(dig_min_buf, &dig_min);

                if (cur_index - dot_index <= 3)
                {
                    strncpy(dig_max_buf, format + last_index + dot_index + 1, cur_index - dot_index - 1);
                    dig_max_buf[cur_index - dot_index - 1] = '\0';
                }
                t_string_to_int(dig_max_buf, &dig_max);
            }

            switch(format[cur_index])
            {
            /* time */
            case 'd':
            {
                cur_index += 2;
                tint temp_index = t_string_find_char(format, cur_index, ')', TRUE);
                /* add time */
                splits->splits[split_count].data = malloc(temp_index + 1);
                if (NULL == splits->splits[split_count].data)
                {
                    goto ERROR;
                }
                strncpy(splits->splits[split_count].data, format + cur_index, temp_index);
                splits->splits[split_count].data[temp_index] = '\0';
                splits->splits[split_count].write_buf = write_time;
                cur_index += temp_index + 1;
            }
                break;
            /* file name */
            case 'f':
                splits->splits[split_count].data = NULL;
                splits->splits[split_count].write_buf = write_filename;
                cur_index ++;
                break;
            /* __FILE__ */
            case 'F':
                splits->splits[split_count].data = NULL;
                splits->splits[split_count].write_buf = write_filename_full;
                cur_index ++;
                break;
            /* __LINE__ */
            case 'L':
                splits->splits[split_count].data = NULL;
                splits->splits[split_count].write_buf = write_line;
                cur_index ++;
                break;
            /* user input message */
            case 'm':
                splits->splits[split_count].data = NULL;
                splits->splits[split_count].write_buf = write_message;
                cur_index ++;
                break;
            /* \n */
            case 'n':
                splits->splits[split_count].data = malloc(2);
                if (NULL == splits->splits[split_count].data)
                {
                    goto ERROR;
                }
                splits->splits[split_count].data[0] = '\n';
                splits->splits[split_count].data[1] = '\0';
                splits->splits[split_count].write_buf = write_direct;
                cur_index ++;
                break;
            /* level:debug */
            case 'V':
                splits->splits[split_count].data = NULL;
                splits->splits[split_count].write_buf = write_level_upper;
                cur_index ++;
                break;
            /* level:DEBUG */
            case 'v':
                splits->splits[split_count].data = NULL;
                splits->splits[split_count].write_buf = write_level_lower;
                cur_index ++;
                break;
            /* error happend */
            default:
                /* should not reach here */
                T_ASSERT(FALSE);
                break;
            }

            splits->splits[split_count].align = align_flag;
            splits->splits[split_count].width_min = dig_min;
            splits->splits[split_count].width_max = dig_max;
            split_count ++;
            last_index = cur_index;
        }
        else
        {
            if (last_index < format_len)
            {
                /* rest is string*/
                splits->splits[split_count].data = malloc(format_len - last_index + 1);
                if (NULL == splits->splits[split_count].data)
                {
                    goto ERROR;
                }
                strcpy(splits->splits[split_count].data, format + last_index);
                splits->splits[split_count].write_buf = write_direct;
            }
            break;
        }
    }

    return splits;

ERROR:
    split_format_free(splits);
    return NULL;
}


/**
 * @brief add format value to hash table
 * @param hash - hash table handle
 * @param name - category name 
 * @param format - format string
 */
static tint add_format(thash_string **hash, const tchar *name, const tchar *format)
{
    T_ASSERT(NULL != hash);
    T_ASSERT(NULL != name);
    T_ASSERT(NULL != format);

    tint err = 0;
    tuint32 split_count = 0;

    if (!format_validation(format, &split_count))
    {
        return -EINVAL;
    }

    format_node *node = malloc(sizeof(format_node));
    if (NULL != node)
    {
        err = t_hash_string_init_node(&node->node, name);
        if (0 == err)
        {
            /* add format */
            node->value = malloc(strlen(format) + 1);;
            if (NULL != node->value)
            {
                strcpy(node->value, format);
                node->splits = format_to_split_quick(format, split_count);
                if (NULL == node->splits)
                {
                    free(node->value);
                    free(node->node.key);
                    free(node);
                    err = -ENOMEM;
                }
            }
            else
            {
                free(node->node.key);
                free(node);
                err = -ENOMEM;
            }
            *hash = t_hash_string_insert(*hash, &node->node);
        }
    }
    else
    {
        err = -ENOMEM;
    }

    return err;
}

/**
 * @brief insert key and value to format hash table
 * @param key - format name string
 * @param value - format string
 * @return error code, 0 means no error
 */
static tint format_process(void *key, void *value, void *userdata)
{
    T_ASSERT(NULL != key);
    T_ASSERT(NULL != value);
    T_ASSERT(NULL != userdata);

    tchar *format = (tchar *)value;
    if ('\"' == *format)
    {
        format++;
    }

    tint len = strlen(format);
    if ('\"' == format[len - 1])
    {
        format[len - 1] = '\0';
    }

    return add_format((thash_string **)userdata, key, format);
}

/**
 * @brief filter output format group
 * @param keyfile - keyfile handle
 * @param hash - formats hash table
 * @return error code, 0 means no error
 */
tint filter_format(const tkeyfile *keyfile, thash_string **hash)
{
    T_ASSERT(NULL != keyfile);
    T_ASSERT(NULL != hash);

    tint err = 0;


    if (0 != t_hash_string_count(*hash))
    {
        t_hash_string_empty(*hash, free_format);
    }

    /* check group exists first */
    if (t_keyfile_contains_group(keyfile, GROUP_NAME_FORMAT))
    {
        err = t_keyfile_group_foreach(keyfile, GROUP_NAME_FORMAT, format_process, hash);
    }
    else
    {
        err = add_format(hash, DEFAULT_FORMAT_NAME, DEFAULT_FORMAT);
    }

    if (0 == err)
    {
        /* add default format */
        if (!t_hash_string_contain(*hash, DEFAULT_FORMAT_NAME))
        {
            err = add_format(hash, DEFAULT_FORMAT_NAME, DEFAULT_FORMAT);
        }
    }

    return err;
}

/**
 * @brief validate format string
 * @param format - format string
 * @param count - output split count if need
 * @return validate result
 */
tbool format_validation(const tchar *format, tuint32 *count)
{
    T_ASSERT(NULL != format);
    tuint32 split_count = 0;
    tint cur_index = 0, last_index = 0;
    tbool ret = TRUE;

    while(1)
    {
        last_index = cur_index;
        cur_index = t_string_find_char(format, last_index, '%', TRUE);
        if (-1 != cur_index)
        {
            if (0 != cur_index)
            {
                split_count ++;
            }
            cur_index += last_index;
            cur_index ++;

            /* skip digital and % */
            if ('-' == format[cur_index])
            {
                cur_index ++;
            }
            else if ('%' == format[cur_index])
            {
                cur_index ++;
                split_count ++;
                continue;
            }

            /* skip digital */
            if ('.' == format[cur_index])
            {
                ret = FALSE;
                break;
            }

            while((format[cur_index] >= '0') &&
                  (format[cur_index] <= '9'))
            {
                cur_index ++;
            }

            if ('.' == format[cur_index])
            {
                cur_index ++;
                if ((format[cur_index] < '0') ||
                    (format[cur_index] > '9'))
                {
                    ret = FALSE;
                    break;
                }
            }

            while((format[cur_index] >= '0') &&
                  (format[cur_index] <= '9'))
            {
                cur_index ++;
            }

            /* validate */
            switch(format[cur_index])
            {
            /* time */
            case 'd':
                cur_index ++;
                if ('(' == format[cur_index])
                {
                    tint temp_index = t_string_find_char(format, cur_index, ')', TRUE);
                    if (-1 == temp_index)
                    {
                        ret = FALSE;
                        break;
                    }
                    else
                    {
                        split_count ++;
                        cur_index += temp_index + 1;
                    }
                }
                else
                {
                    ret = FALSE;
                }
                break;
            /* file name */
            case 'f':
            /* __FILE__ */
            case 'F':
            /* __LINE__ */
            case 'L':
            /* user input message */
            case 'm':
            /* \n */
            case 'n':
            /* level:debug */
            case 'V':
            /* level:DEBUG */
            case 'v':
                split_count ++;
                cur_index ++;
                break;
            /* error happend */
            default:
                ret = FALSE;
                break;
            }
        }
        else
        {
            if (last_index < strlen(format))
            {
                split_count ++;
            }
            break;
        }

        if (!ret)
        {
            split_count = 0;
            break;
        }
    }

    if (NULL != count)
    {
        *count = split_count;
    }

    return ret;
}

/**
 * @brief free split format memory space
 * @param split - split format handle
 * @param length - split format length
 */
void split_format_free(split_format *split)
{
    T_ASSERT(NULL != split);
    for (tuint32 i = 0; i < split->count; ++i)
    {
        if (NULL != split->splits[i].data)
        {
            free(split->splits[i].data);
        }
    }

    free(split->splits);
    free(split);
}


/**
 * @brief split format
 * @param format - format string
 * @return split format pointer
 */
split_format *format_to_split(const tchar *format)
{
    T_ASSERT(NULL != format);

    tuint32 split_count = 0;

    if (format_validation(format, &split_count))
    {
        return format_to_split_quick(format, split_count);
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief convert split format to string
 * @param buf - string output buffer
 * @param split - split format handle
 * @param pre - preprocess information
 */
void format_split_to_string(tchar *buf, const split_format *splits, const preprocess_info *pre)
{
    T_ASSERT(NULL != splits);
    T_ASSERT(NULL != buf);
    tuint32 written_len = 0;
    for (tuint32 i = 0; i < splits->count; ++i)
    {
        written_len = splits->splits[i].write_buf(buf, &splits->splits[i], pre);
        buf += written_len;
    }
}


