/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "tlog/tlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

/* thread category */
static const tlog_category *cat = NULL;

/* configure buffer */
static const char thread_cfg[] = "[rules]\nfile.*=/tmp/tlog_thread.log\n";

static void *log_thread1(void *arg)
{
    int count = 0;
    while(1)
    {
        tlog_debug(cat, "aaaaaa");
        count ++;
        if (count > 1000000)
        {
            pthread_exit((void *)0);
        }
    }
}

static void *log_thread2(void *arg)
{
    int count = 0;
    while(1)
    {
        tlog_debug(cat, "bbbbbb");
        count ++;
        if (count > 1000000)
        {
            pthread_exit((void *)0);
        }
    }
}

static void *log_thread3(void *arg)
{
    int count = 0;
    while(1)
    {
        tlog_debug(cat, "cccccc");
        count ++;
        if (count > 1000000)
        {
            pthread_exit((void *)0);
        }
    }
}

int main(int argc, char **argv)
{
    int err = tlog_open(thread_cfg, TLOG_MEM);
    if (0 != err)
    {
        fprintf(stdout, "\e[31mopen tlog failed: %s\e[0m\n", strerror(-err));
        exit(1);
    }

    cat = tlog_get_category("file");

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, log_thread1, NULL);
    pthread_create(&tid2, NULL, log_thread2, NULL);
    pthread_create(&tid3, NULL, log_thread3, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);


    tlog_close();

    return 0;
}

