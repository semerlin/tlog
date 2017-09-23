#include "gtest/gtest.h"
#include "../src/tkeyfile.h"

char filename[128] = {0};

#ifdef T_ENABLE_ASSERT
TEST(TkeyfileTest, Death)
{
    EXPECT_DEATH(t_keyfile_free(NULL), "");
    EXPECT_DEATH(t_keyfile_load_from_file(NULL, NULL), "");
    EXPECT_DEATH(t_keyfile_get_string(NULL, NULL, NULL, NULL, NULL), "");
    EXPECT_DEATH(t_keyfile_get_int(NULL, NULL, NULL, 0), "");
    EXPECT_DEATH(t_keyfile_get_bool(NULL, NULL, NULL, true), "");
    EXPECT_DEATH(t_keyfile_has_group(NULL, NULL), "");
    EXPECT_DEATH(t_keyfile_has_key(NULL, NULL, NULL), "");
    EXPECT_DEATH(t_keyfile_ref(NULL), "");
    EXPECT_DEATH(t_keyfile_unref(NULL), "");
}

#endif

TEST(TkeyfileTest, Function)
{
    tkeyfile *keyfile = t_keyfile_new();
    ASSERT_NE((void *)0, keyfile);
    int ret = t_keyfile_load_from_file(keyfile, filename);
    ASSERT_EQ(0, ret);
    EXPECT_TRUE(t_keyfile_has_group(keyfile, "test1"));
    EXPECT_TRUE(t_keyfile_has_group(keyfile, "test2"));
    EXPECT_FALSE(t_keyfile_has_group(keyfile, "test"));
    EXPECT_TRUE(t_keyfile_has_key(keyfile, "test1", "flag3"));
    char out[12];

    t_keyfile_get_string(keyfile, "test1", "flag1", out, "sss");
    EXPECT_STREQ("a", out);
    t_keyfile_get_string(keyfile, "test1", "flag2", out, "sss");
    EXPECT_STREQ("b", out);
    t_keyfile_get_string(keyfile, "test1", "flag3", out, "sss");
    EXPECT_STREQ("c", out);
    t_keyfile_get_string(keyfile, "test1", "flag4", out, "sss");
    EXPECT_STREQ("d", out);
    EXPECT_EQ(8, t_keyfile_get_int(keyfile, "test1", "flag4", 8));
    EXPECT_TRUE(t_keyfile_get_bool(keyfile, "test1", "flag5", false));
    EXPECT_EQ(234, t_keyfile_get_int(keyfile, "test1", "flag6", 8));

    t_keyfile_get_string(keyfile, "test2", "flag1", out, "sss");
    EXPECT_STREQ("e", out);
    t_keyfile_get_string(keyfile, "test2", "flag2", out, "sss");
    EXPECT_STREQ("f", out);
    EXPECT_FALSE(t_keyfile_get_bool(keyfile, "test2", "flag3", true));
    EXPECT_EQ(-22, t_keyfile_get_int(keyfile, "test2", "flag4", 8));

    t_keyfile_free(keyfile);
}



int main(int argc, char **argv)
{
    if (argc > 1)
    {
        strcpy(filename, argv[1]);
    }
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
