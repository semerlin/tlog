#include "gtest/gtest.h"
#include "../src/tshareptr.h"

#ifdef T_ENABLE_ASSERT
TEST(TShareptrTest, Death)
{
    tshareptr *test_ptr = (tshareptr *)malloc(sizeof(tshareptr));
    EXPECT_DEATH(t_shareptr_init(NULL), "");
    EXPECT_DEATH(t_shareptr_ref(NULL), "");
    EXPECT_DEATH(t_shareptr_unref(NULL), "");
    EXPECT_DEATH(t_shareptr_ref(test_ptr), "");
    EXPECT_DEATH(t_shareptr_unref(test_ptr), "");
}
#endif

TEST(TShareptrTest, RefAndUnref)
{
    tshareptr *test_ptr = (tshareptr *)malloc(sizeof(tshareptr));
    t_shareptr_init(test_ptr);
    EXPECT_EQ(1, test_ptr->ref_count);
    t_shareptr_ref(test_ptr);
    t_shareptr_ref(test_ptr);
    EXPECT_EQ(3, test_ptr->ref_count);
    t_shareptr_unref(test_ptr);
    EXPECT_EQ(2, test_ptr->ref_count);
    t_shareptr_unref(test_ptr);
    t_shareptr_unref(test_ptr);
    //EXPECT_DEATH(t_shareptr_unref(test_ptr), "");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

