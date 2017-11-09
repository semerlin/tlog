/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "tlog/tlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int err = tlog_open("./file.conf", TLOG_FILE);
    if (0 != err)
    {
        fprintf(stderr, "open tlog failed: \"file.conf\" %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }


    const tlog_category *file = tlog_get_category("file");
    if (NULL == file)
    {
        fprintf(stderr, "missing category \"file\"\n");
        exit(EXIT_FAILURE);
    }
    tlog_debug(file, "this is file example");

    exit(EXIT_SUCCESS);
}
