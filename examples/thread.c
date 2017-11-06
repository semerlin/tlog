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
#include <signal.h>

#define DEFAULT_TIME  5

/* thread category */
static const tlog_category *cat = NULL;

/* configure buffer */
static const char thread_cfg[] = "[format]\nfmt=\"%d(%F %T) %-6V [tid:%t] [%f:%L:%U] %m%n\"\n[rules]\nfile.*=fmt;/tmp/tlog_thread.log\n";


static pthread_t tid1, tid2, tid3;


/* timeout signal process */
static void alarm_act(int sig)
{
    pthread_cancel(tid1);
    pthread_cancel(tid2);
    pthread_cancel(tid3);
}


static void *log_thread1(void *arg)
{
    while(1)
    {
        tlog_debug(cat, "this is thread1");
    }
    pthread_exit((void *)0);
}

static void *log_thread2(void *arg)
{
    while(1)
    {
        tlog_debug(cat, "this is thread2");
    }
    pthread_exit((void *)0);
}

static void *log_thread3(void *arg)
{
    while(1)
    {
        tlog_debug(cat, "this is thread3");
    }
    pthread_exit((void *)0);
}

int main(int argc, char **argv)
{
    unsigned int time = DEFAULT_TIME;
    char *endptr = NULL;
    int opt;
    long val = 0;
    /* get -t option */
    while (-1 != (opt = getopt(argc, argv, "t:")))
    {
        switch (opt)
        {
        case 't':
            val = strtol(optarg, &endptr, 10);
            if ((0 == errno) && (endptr != optarg))
            {
                time = val;
            }
            break;
        default:
            fprintf(stderr, "Usage: %s [-t nsecs]\n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    /* regiter alarm signal */
    struct sigaction act;
    act.sa_handler = &alarm_act;
    act.sa_flags = SA_ONSTACK;
    sigaction(SIGALRM, &act, 0);

    /* init log system */
    int err = tlog_open(thread_cfg, TLOG_MEM);
    if (0 != err)
    {
        fprintf(stdout, "\e[31mopen tlog failed: %s\e[0m\n", strerror(-err));
        exit(EXIT_FAILURE);
    }

    cat = tlog_get_category("file");

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

    alarm(time);

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

