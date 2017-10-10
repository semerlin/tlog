#include "gtest/gtest.h"
#include "../src/format.h"
#include "../src/tkeyfile.h"

char filename[128] = {0};

#ifdef T_ENABLE_ASSERT
TEST(FormatTest, Death)
{
    ASSERT_DEATH(filter_format(NULL, NULL), "");
    ASSERT_DEATH(get_format(NULL, NULL), "");
}
#endif

TEST(FormatTest, Convert)
{
    tkeyfile *keyfile = t_keyfile_new();
    ASSERT_NE((void *)0, keyfile);
    t_keyfile_use_last_sep(TRUE);
    ASSERT_EQ(0, t_keyfile_load_from_file(keyfile, filename));
    thash_string *format = format_new();
    ASSERT_NE((void *)0, format);
    filter_format(keyfile, format);

    EXPECT_STREQ("[%t]", get_format(format, "default"));
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

