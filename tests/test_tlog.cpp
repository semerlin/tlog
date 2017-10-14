/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "gtest/gtest.h"
#include "../include/tlog/tlog.h"
#include <time.h>

char filename[128] = {0};

TEST(TlogTest, Function)
{
    ASSERT_EQ(0, tlog_init_from_file(filename));
    const tlog_category *cat1 = tlog_get_category("test1");
    const tlog_category *cat2 = tlog_get_category("test2");
    const tlog_category *cat3 = tlog_get_category("test3");
    ASSERT_NE((void *)0, cat1);
    ASSERT_NE((void *)0, cat2);
    ASSERT_NE((void *)0, cat3);

    tlog_debug(cat1, "this is debug");
    tlog_info(cat1, "this is info");
    tlog_error(cat1, "this is error");

    tlog_warn(cat2, "this is warn");
    tlog_fatal(cat3, "this is fatal");
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
