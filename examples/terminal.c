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
    int err = tlog_open("./terminal.conf", TLOG_FILE);
    if (0 != err)
    {
        fprintf(stderr, "open tlog failed: \"terminal.conf\" %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }


    const tlog_category *terminal = tlog_get_category("terminal");
    if (NULL == terminal)
    {
        fprintf(stderr, "missing category \"terminal\"\n");
        exit(EXIT_FAILURE);
    }
    tlog_debug(terminal, "this is terminal example");

    exit(EXIT_SUCCESS);
}
