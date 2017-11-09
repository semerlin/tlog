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
#include <unistd.h>


/* thread category */
static const tlog_category *cat = NULL;


static void *log_thread1(void *arg)
{
    tlog_put_mdc("name", "thread1");
    tlog_debug(cat, "this is: ");
    pthread_exit((void *)0);
}

static void *log_thread2(void *arg)
{
    tlog_put_mdc("name", "thread2");
    tlog_debug(cat, "this is: ");
    pthread_exit((void *)0);
}

static void *log_thread3(void *arg)
{
    tlog_put_mdc("name", "thread3");
    tlog_debug(cat, "this is: ");
    pthread_exit((void *)0);
}

int main(int argc, char **argv)
{
    /* init log system */
    int err = tlog_open("./mdc.conf", TLOG_FILE);
    if (0 != err)
    {
        fprintf(stdout, "open tlog failed: \"mdc.conf\" %s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    cat = tlog_get_category("mdc");

    pthread_t tid1, tid2, tid3;

    if (0 != (err = pthread_create(&tid1, NULL, log_thread1, NULL)))
    {
        fprintf(stderr, "create thread1 failed: \%s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if (0 != (err = pthread_create(&tid2, NULL, log_thread2, NULL)))
    {
        fprintf(stderr, "create thread2 failed: \%s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if (0 != (err = pthread_create(&tid3, NULL, log_thread3, NULL)))
    {
        fprintf(stderr, "create thread3 failed: \%s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if (0 != (err = pthread_join(tid1, NULL)))
    { 
        fprintf(stderr, "join thread1 failed: \%s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if (0 != (err = pthread_join(tid2, NULL)))
    {
        fprintf(stderr, "join thread2 failed: \%s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    if (0 != (err = pthread_join(tid3, NULL)))
    {
        fprintf(stderr, "join thread3 failed: \%s\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    tlog_close();

    exit(EXIT_SUCCESS);
}

