/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "gtest/gtest.h"
#include "../src/tassert.h"

#ifdef T_ENABLE_ASSERT
int AssertFunc(int val)
{
    T_ASSERT(val == 3);

    return 2;
}

TEST(TAssertTest, Death)
{
    ASSERT_DEATH(AssertFunc(2), "");
    ASSERT_EQ(2, AssertFunc(3));
}
#endif


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

