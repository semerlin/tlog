#include "gtest/gtest.h"
#include "../src/tassert.h"

int AssertFunc(int val)
{
    T_ASSERT(val == 3);

    return 2;
}

TEST(AssertTest, Assert)
{
    ASSERT_DEATH(AssertFunc(2), "");
    ASSERT_EQ(2, AssertFunc(3));
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

