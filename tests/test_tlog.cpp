/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "gtest/gtest.h"
#include "../include/tlog/tlog.h"
#include <time.h>

char filename[128] = {0};

const tlog_category *cat5 = NULL;


static void *mdc_thread1(void *arg)
{
    tlog_put_mdc("thread_msg", "thread1");
    tlog_debug(cat5, "this is thread:");
    pthread_exit((void *)0);
}

static void *mdc_thread2(void *arg)
{
    tlog_put_mdc("thread_msg", "thread2");
    tlog_debug(cat5, "this is thread:");
    pthread_exit((void *)0);
}

static void *mdc_thread3(void *arg)
{
    tlog_put_mdc("thread_msg", "thread3");
    tlog_debug(cat5, "this is thread:");
    pthread_exit((void *)0);
}

TEST(TlogTest, Function)
{
    ASSERT_EQ(0, tlog_open(filename, TLOG_FILE));
    const tlog_category *cat1 = tlog_get_category("test1");
    const tlog_category *cat2 = tlog_get_category("test2");
    const tlog_category *cat3 = tlog_get_category("test3");
    const tlog_category *cat4 = tlog_get_category("test4");
    cat5 = tlog_get_category("test5");
    ASSERT_NE((void *)0, cat1);
    ASSERT_NE((void *)0, cat2);
    ASSERT_NE((void *)0, cat3);
    ASSERT_NE((void *)0, cat4);
    ASSERT_NE((void *)0, cat5);

    tlog_debug(cat1, "this is debug");
    tlog_info(cat1, "this is info");
    tlog_error(cat1, "this is error");

    tlog_warn(cat2, "this is warn");
    tlog_fatal(cat3, "this is fatal");

    tlog_notice(cat4, "this is notice");

    pthread_t tid1, tid2, tid3;
    tlog_put_mdc("thread_msg", "thread0");
    pthread_create(&tid1, NULL, mdc_thread1, NULL);
    pthread_create(&tid2, NULL, mdc_thread2, NULL);
    pthread_create(&tid3, NULL, mdc_thread3, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    tlog_debug(cat5, "this is thread:");
}


int main(int argc, char **argv)
{
    if (argc > 1)
    {
        strcpy(filename, argv[1]);
    }
 
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
