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
            value = kv[1]
            if group_name == "general":
                pass
            elif group_name == "format":
                pass
            else:
                pass


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


