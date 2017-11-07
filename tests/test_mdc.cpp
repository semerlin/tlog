/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include <pthread.h>
#include "gtest/gtest.h"
#include "../src/mdc.h"
#include "../src/tkeyfile.h"
#include "../src/global.h"


mdc *g_mdc = NULL;
char *value1, *value2, *value3;

static void *mdc_thread1(void *arg)
{
    mdc_put(g_mdc, "a1", "thread1");
    value1 = mdc_get(g_mdc, "a1");
    pthread_exit((void *)0);
}

static void *mdc_thread2(void *arg)
{
    mdc_put(g_mdc, "a1", "thread2");
    value2 = mdc_get(g_mdc, "a1");
    pthread_exit((void *)0);
}

static void *mdc_thread3(void *arg)
{
    mdc_put(g_mdc, "a1", "thread3");
    value3 = mdc_get(g_mdc, "a1");
    pthread_exit((void *)0);
}

#ifdef T_ENABLE_ASSERT
TEST(MdcTest, Death)
{
    ASSERT_DEATH(mdc_free(NULL), "");
    ASSERT_DEATH(mdc_clean(NULL), "");
    ASSERT_DEATH(mdc_put(NULL, NULL, NULL), "");
    ASSERT_DEATH(mdc_get(NULL, NULL), "");
    ASSERT_DEATH(mdc_remove(NULL, NULL), "");
}
#endif

TEST(MdcTest, Mdc)
{
    mdc *mdc_map= mdc_new();
    ASSERT_NE((void *)0, mdc_map);
    ASSERT_EQ(0, mdc_put(mdc_map, "a1", "b1"));
    ASSERT_EQ(0, mdc_put(mdc_map, "a2", "b2"));
    ASSERT_EQ(0, mdc_put(mdc_map, "a3", "b3"));
    ASSERT_STREQ("b1", mdc_get(mdc_map, "a1"));
    ASSERT_STREQ("b2", mdc_get(mdc_map, "a2"));
    ASSERT_STREQ("b3", mdc_get(mdc_map, "a3"));
    ASSERT_EQ(0, mdc_put(mdc_map, "a1", "b4"));
    ASSERT_STREQ("b4", mdc_get(mdc_map, "a1"));
    mdc_remove(mdc_map, "a1");
    ASSERT_STREQ((char *)0, mdc_get(mdc_map, "a1"));
    mdc_remove(mdc_map, "a1");
    mdc_remove(mdc_map, "a2");
    mdc_remove(mdc_map, "a3");
    mdc_remove(mdc_map, "a4");
    ASSERT_STREQ((char *)0, mdc_get(mdc_map, "a4"));
}

TEST(MdcTest, MultiThread)
{
    pthread_t tid1, tid2, tid3;
    g_mdc = mdc_new();
    ASSERT_NE((void *)0, g_mdc);
    ASSERT_EQ(0, mdc_put(g_mdc, "a1", "thread0"));
    pthread_create(&tid1, NULL, mdc_thread1, NULL);
    pthread_create(&tid2, NULL, mdc_thread2, NULL);
    pthread_create(&tid3, NULL, mdc_thread3, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    ASSERT_STREQ("thread0", mdc_get(g_mdc, "a1"));
    ASSERT_STREQ("thread1", value1);
    ASSERT_STREQ("thread2", value2);
    ASSERT_STREQ("thread3", value3);
}



int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

