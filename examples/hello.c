#include "tlog/tlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int err = tlog_open("./hello.conf", TLOG_FILE);
    if (0 != err)
    {
        fprintf(stderr, "open tlog failed: \"hello.conf\" %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }


    const tlog_category *hello = tlog_get_category("hello");
    if (NULL == hello)
    {
        fprintf(stderr, "missing category \"hello\"\n");
        exit(EXIT_FAILURE);
    }
    tlog_debug(hello, "hello world");

    exit(EXIT_SUCCESS);
}
