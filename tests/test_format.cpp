#include "gtest/gtest.h"
#include "../src/format.h"
#include "../src/tkeyfile.h"
#include "../src/global.h"

char filename[128] = {0};

#ifdef T_ENABLE_ASSERT
TEST(FormatTest, Death)
{
    ASSERT_DEATH(filter_format(NULL, NULL), "");
    ASSERT_DEATH(get_format(NULL, NULL), "");
}
#endif

TEST(FormatTest, Format)
{
    tkeyfile *keyfile = t_keyfile_new();
    ASSERT_NE((void *)0, keyfile);
    t_keyfile_use_last_sep(TRUE);
    ASSERT_EQ(0, t_keyfile_load_from_file(keyfile, filename));
    thash_string *format = format_new();
    ASSERT_NE((void *)0, format);

    ASSERT_EQ(0, filter_format(keyfile, &format));

    EXPECT_STREQ(DEFAULT_FORMAT, get_format(format, "default"));
    EXPECT_STREQ("%d(%y-%m-%n %T)", get_format(format, "simple"));
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
