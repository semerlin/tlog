/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "tlog/tlog.h"
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/utsname.h>
#include <string.h>


#define DEFAULT_TIME  5

static unsigned char is_testing = false;

/* configure buffer */
static const char benchmark_cfg[] = "[general]\n[format]\nstdfmt=\"%d(%Y-%m-%d %T) %-6V [%F:%L] %m\"\n[rules]\nstd.*=stdfmt;>stdout\nfile.*=/tmp/tlog_benchmark.log\npipe.*=| cat > /dev/null\n";

/* timeout signal process */
static void alarm_act(int sig)
{
    is_testing = false;
}


/**
 * @brief init benchmark
 */
static void benchmark_init(void)
{
    /* regiter alarm signal */
    struct sigaction act;
    act.sa_handler = &alarm_act;
    sigaction(SIGALRM, &act, 0);

    /* print test environment */
    fprintf(stdout, "\e[32mtest environment:\e[0m\n");
    struct utsname name;
    if (0 == uname(&name))
    {
        fprintf(stdout, "system:  %s\n", name.sysname);
        fprintf(stdout, "release: %s\n", name.release);
        fprintf(stdout, "version: %s\n", name.version);
        fprintf(stdout, "machine: %s\n", name.machine);
    }
}

static int calc_speed(const struct timeval *start, 
        const struct timeval *stop, double count)
{
    if (0 == count)
    {
        return 0;
    }

    double secs = difftime(stop->tv_sec, start->tv_sec);
    return count * 1000000 / (secs * 1000000 + (double)(stop->tv_usec - start->tv_usec));
}

static void internal_test(unsigned int time, const tlog_category *cat, 
        struct timeval *start, struct timeval *stop, double *count)
{
    *count = 0;

    is_testing = true;
    alarm(time);
    /* record start time */
    gettimeofday(start, NULL);
    while(is_testing)
    {
        tlog_debug(cat, "this is tlog speed test\r");
        *count += 1;
    }
    /* record stop time */
    gettimeofday(stop, NULL);
}


static void printf_benchmark_result(const struct timeval *start, 
        const struct timeval *stop, double count)
{
    struct tm begin_tm, end_tm;
    char time_buf[32];
    /* print bench mark result */
    fprintf(stdout, "-------------------------------------------------------------------------------------\n");
    fprintf(stdout, "|         start time         |         stop time          |      average speed      |\n");
    fprintf(stdout, "|----------------------------|----------------------------|-------------------------|\n");
    fprintf(stdout, "| ");
    localtime_r(&start->tv_sec, &begin_tm);
    strftime(time_buf, 32, "%Y-%m-%d %T", &begin_tm);
    fprintf(stdout, "%s.%06d", time_buf, (int)(start->tv_usec));
    fprintf(stdout, " | ");
    localtime_r(&stop->tv_sec, &end_tm);
    strftime(time_buf, 32, "%Y-%m-%d %T", &end_tm);
    fprintf(stdout, "%s.%06d", time_buf, (int)(stop->tv_usec));
    fprintf(stdout, " | ");
    fprintf(stdout, "   %07d logs/secs    |\n", calc_speed(start, stop, count));
    fprintf(stdout, "-------------------------------------------------------------------------------------\n");
}

static void test_stdout(unsigned int time)
{
    fprintf(stdout, "\e[32mtesting stdout speed....\e[0m\n");
    const tlog_category *cat_std = tlog_get_category("std");
    struct timeval tv_begin, tv_end;
    double count = 0;

    /* run test */
    internal_test(time, cat_std, &tv_begin, &tv_end, &count);
    fprintf(stdout, "\n");

    /* print bench mark result */
    printf_benchmark_result(&tv_begin, &tv_end, count);
}


static void test_file(unsigned int time)
{
    fprintf(stdout, "\e[32mtesting file speed....\e[0m\n");
    const tlog_category *cat_file = tlog_get_category("file");
    struct timeval tv_begin, tv_end;
    double count = 0;

    /* run test */
    internal_test(time, cat_file, &tv_begin, &tv_end, &count);

    /* print bench mark result */
    printf_benchmark_result(&tv_begin, &tv_end, count);
}



static void test_pipeline(unsigned int time)
{
    fprintf(stdout, "\e[32mtesting pipeline speed....\e[0m\n");
    const tlog_category *cat_pipe = tlog_get_category("pipe");
    struct timeval tv_begin, tv_end;
    double count = 0;

    /* run test */
    internal_test(time, cat_pipe, &tv_begin, &tv_end, &count);

    /* print bench mark result */
    printf_benchmark_result(&tv_begin, &tv_end, count);
}

int main(int argc, char **argv)
{
    unsigned int time = DEFAULT_TIME;
    if (argc > 1)
    {
        time = atoi(argv[1]);
        if (time <= 0)
        {
            time = DEFAULT_TIME;
        }
#if 0
        /* TODO find this code core dump on 32bits machine reason */
        char *endptr = NULL, *str = NULL;
        long val = 0;
        str = argv[1];
        val = strtol(str, &endptr, 10);
        if ((0 == errno) && (endptr != str))
        {
            time = val;
        }
#endif
    }

    benchmark_init();
    fprintf(stdout, "\n");
    int err = tlog_open(benchmark_cfg, TLOG_MEM);
    if (0 != err)
    {
        fprintf(stdout, "\e[31mopen tlog failed: %s\e[0m\n", strerror(-err));
        exit(1);
    }

    test_pipeline(time);
    test_stdout(time);
    test_file(time);

    return 0;
}
