#!/usr/bin/env python

#############################################################################
# Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
#
# See the COPYING file for the terms of usage and distribution.
#############################################################################

# -*- coding: UTF-8 -*-

import sys

in_group = False
group_name = ""
formats = []

def printinfo(level, line, string, info):
    print("[%s : %d] %s: %s" % (level, line, string, info))

 
def parse_group(line, data):
    start_index = data.find('[')
    end_index = 0
    if start_index == -1:
        printinfo("error", line, data, "invalid group, missing \'[\'")
    else:
        end_index = data.find(']')
        if end_index == -1:
            printinfo("error", line, data, "invalid group, missing \']\'")
        else:
            #check useless data
            if data.startswith('[') == False:
                printinfo("error", line, data, "useless string before \'[\'")
            else:
                if data.endswith(']') == False:
                    printinfo("error", line, data, "useless string after \']\'")
                else:
                    global in_group
                    in_group = True
                    global group_name
                    group_name = data[start_index + 1 : end_index]

                    #check group name 
                    if group_name == "general" or group_name == "format" or group_name == "rules":
                        pass
                    else:
                        printinfo("warning", line, data, "unkown group %s" % group_name)

def format_validation(line, line_data, data): 
    cur_index = 0
    last_index = 0
    num = 0
    while 1:
        last_index = cur_index;
        cur_index = data.find('%', last_index)

        if -1 != cur_index:
            cur_index += 1

            # skip digital and %
            if '-' == data[cur_index]:
                cur_index += 1
            elif '%' == data[cur_index]:
                cur_index += 1
                continue

            # skip digital
            if '.' == data[cur_index]:
                printinfo("error", line, line_data, "need number after \'-\'")
                break

            while data[cur_index] >= '0' and data[cur_index] <= '9':
                cur_index += 1

            if '.' == data[cur_index]:
                cur_index += 1
                if data[cur_index] < '0' or data[cur_index] > '9':
                    printinfo("error", line, line_data, "need number after \'.\'")
                    break

                while data[cur_index] >= '0' and data[cur_index] <= '9':
                    cur_index += 1
            # validate
            if data[cur_index] == 'd':
                cur_index += 1
                if '(' == data[cur_index]:
                    temp_index = data.find(')', cur_index)
                    if -1 == temp_index:
                        printinfo("error", line, line_data, "missing \')\' after \'%d\'")
                    else:
                        cur_index = temp_index + 1
                else:
                    printinfo("error", line, line_data, "missing \'(\' after \'%d\'")
            elif data[cur_index] == 'f' or \
                 data[cur_index] == 'F' or \
                 data[cur_index] == 'L' or \
                 data[cur_index] == 'U' or \
                 data[cur_index] == 'm' or \
                 data[cur_index] == 'n' or \
                 data[cur_index] == 'V' or \
                 data[cur_index] == 'v' or \
                 data[cur_index] == 'S' or \
                 data[cur_index] == 'M':
                cur_index += 1
            else:
                printinfo("error", line, line_data, "unknown command \'%%%s\'" % data[cur_index])
        else:
            break


def output_validation(line, line_data, data):
    if '>' == data[0]:
        if ">stdout" == data or ">stderr" == data:
            pass
        else:
            printinfo("error", line, line_data, "unknown output \'%s\'" % data)
    elif '|' == data[0]:
        index = 1
        while index < len(data):
            if ' ' == data[index]:
                index += 1

        if index >= len(data):
            printinfo("error", line, line_data, "need pipeline output path after \'|\'")
        else:
            pass
    else:
        index = 0
        while index < len(data):
            if '%' != data[index]:
                index += 1
            else:
                index += 1
                if 'd' == data[index]:
                    index += 1
                    if '(' == data[index]:
                        index += 1
                        cur_index = data.find(')', index)
                        if -1 != cur_index:
                            index = cur_index + 1
                        else:
                            printinfo("error", line_data, data, "missing \')\' after %%")
                            break
                    else:
                        printinfo("error", line_data, data, "missing \'(\' after %%d")
                        break
                else:
                    printinfo("error", line, data, "unknown command \'%%s\'" % data[index])
                    break

def is_valid_format(value):
    global formats
    for fmt in formats:
        if fmt == value:
            return True

    return False

    

def rules_validation(line, data, key, value):
    if key.find('.', 0) != -1:
        kv = key.split('.', 2)
        level = kv[1]
    else:
        level = '*'

    level = level.strip()
    level = level.lower()
    if level == 'debug' or \
       level == 'info' or \
       level == 'notice' or \
       level == 'warn' or \
       level == 'error' or \
       level == 'fatal' or \
       level == '*':
       pass
    else:
        printinfo("error", line, data, "unknown level \'%s\'" % level)

    fmt = ""
    output = ""
    if value.find(';', 0) != -1:
        kv = value.split(';', 2)
        fmt = kv[0]
        output = kv[1]
    else:
        fmt = "default"
        output = value

    fmt = fmt.strip()
    output = output.strip()
    global formats
    formats.append("default")
    if not is_valid_format(fmt):
        printinfo("error", line, data, "unknown format \'%s\'" % fmt)
        
    output_validation(line, data, output)



def parse_kv(line, data):
    #check if is group
    if data.startswith('[') and data.endswith(']'):
        parse_group(line, data)
    else:
        index = data.find('=')
        if index == -1:
            printinfo("error", line, data, "missing \'=\'")
        else:
            kv = data.split('=', 2)
            key = kv[0]
            key = key.strip()
            value = kv[1]
            value = value.strip()
            if group_name == "general":
                pass
            elif group_name == "format":
                format_validation(line, data, value)
                global formats
                formats.append(key)
            else:
                rules_validation(line, data, key, value)


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print('Usage: %s %s' % (sys.argv[0], '<configure file>'))
        sys.exit(1)

    file_name = sys.argv[1];
    try:
        fd = open(file_name, "r")
        line_num = 0
        for line in fd.readlines():
            line_num += 1
            line = line.strip()
            if len(line) == 0 or line.startswith('#'):
                continue
            else:
                if in_group == True:
                    parse_kv(line_num, line)
                else:
                    parse_group(line_num, line)

    except Exception as e:
        print str(e)
        sys.exit(1)
    finally:
        if fd:
            fd.close()


