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
    int err = tlog_open("./pipeline.conf", TLOG_FILE);
    if (0 != err)
    {
        fprintf(stderr, "open tlog failed: \"pipeline.conf\" %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }


    const tlog_category *pipeline = tlog_get_category("pipeline");
    if (NULL == pipeline)
    {
        fprintf(stderr, "missing category \"pipeline\"\n");
        exit(EXIT_FAILURE);
    }
    tlog_debug(pipeline, "this is pipeline example");

    exit(EXIT_SUCCESS);
}
