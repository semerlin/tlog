/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "gtest/gtest.h"
#include "../src/tstring.h"


#ifdef T_ENABLE_ASSERT
TEST(TstringTest, Death)
{
    EXPECT_DEATH(t_string_left(NULL, 0, NULL), "");
    EXPECT_DEATH(t_string_right(NULL, 0, NULL), "");
    EXPECT_DEATH(t_string_mid(NULL, 0, 0, NULL), "");
    EXPECT_DEATH(t_string_contain_char(NULL, 0, FALSE), "");
    EXPECT_DEATH(t_string_find_char(NULL, 0, 0, FALSE), "");
    EXPECT_DEATH(t_string_find_char_reverse(NULL, 0, 0, FALSE), "");
    EXPECT_DEATH(t_string_char_count(NULL, 0, 0, FALSE), "");
    EXPECT_DEATH(t_string_contain_string(NULL, NULL, FALSE), "");
    EXPECT_DEATH(t_string_find_string(NULL, 0, NULL, FALSE), "");
    EXPECT_DEATH(t_string_find_string_reverse(NULL, 0, NULL, FALSE), "");
}
#endif

TEST(TstringTest, Truncate)
{
    char test_str[] = "abcabc123abcdef12345acbkkkkk";
    char out_str[64] = {0};

    t_string_left(test_str, 4, out_str);
    EXPECT_STREQ("abca", out_str);
    t_string_left(test_str, 50, out_str);
    EXPECT_STREQ("abcabc123abcdef12345acbkkkkk", out_str);
    t_string_left(test_str, 22, out_str);
    EXPECT_STREQ("abcabc123abcdef12345ac", out_str);

    t_string_right(test_str, 4, out_str);
    EXPECT_STREQ("kkkk", out_str);
    t_string_right(test_str, 50, out_str);
    EXPECT_STREQ("abcabc123abcdef12345acbkkkkk", out_str);
    t_string_right(test_str, 22, out_str);
    EXPECT_STREQ("123abcdef12345acbkkkkk", out_str);

    t_string_mid(test_str, 4, 10, out_str);
    EXPECT_STREQ("bc123abcde", out_str);
    t_string_mid(test_str, 3, 50, out_str);
    EXPECT_STREQ("abc123abcdef12345acbkkkkk", out_str);
    t_string_mid(test_str, 4, 10, out_str);
    EXPECT_STREQ("bc123abcde", out_str);

}

TEST(TstringTest, Contain)
{
    char test_str[] = "abcabc123abcdef12345acbkkkkk";

    EXPECT_TRUE(t_string_contain_char(test_str, 'a', TRUE));
    EXPECT_TRUE(t_string_contain_char(test_str, 'A', FALSE));
    EXPECT_FALSE(t_string_contain_char(test_str, 'i', TRUE));
    EXPECT_FALSE(t_string_contain_char(test_str, 'I', FALSE));

    EXPECT_TRUE(t_string_contain_string(test_str, "def", TRUE));
    EXPECT_TRUE(t_string_contain_string(test_str, "DeF", FALSE));
    EXPECT_TRUE(t_string_contain_string(test_str, "123", TRUE));
    EXPECT_TRUE(t_string_contain_string(test_str, "ABC", FALSE));
    EXPECT_FALSE(t_string_contain_string(test_str, "abc2", TRUE));

    EXPECT_EQ(4, t_string_char_count(test_str, 0, 'a', FALSE));
}

TEST(TstringTest, Find)
{
    char test_str[] = "abcabc123abcdef12345acbkkkkk";
    EXPECT_EQ(0, t_string_find_char(test_str, 0, 'a', TRUE));
    EXPECT_EQ(3, t_string_find_char(test_str, 1, 'a', TRUE));
    EXPECT_EQ(14, t_string_find_char(test_str, 0, 'F', FALSE));
    EXPECT_EQ(6, t_string_find_char(test_str, 0, '1', TRUE));
    EXPECT_EQ(-1, t_string_find_char(test_str, 0, 'X', TRUE));

    EXPECT_EQ(0, t_string_find_char_reverse(test_str, 0, 'a', TRUE));
    EXPECT_EQ(9, t_string_find_char_reverse(test_str, 18, 'a', TRUE));
    EXPECT_EQ(0, t_string_find_char_reverse(test_str, 0, 'A', FALSE));
    EXPECT_EQ(-1, t_string_find_char_reverse(test_str, 0, 'X', TRUE));

    EXPECT_EQ(0, t_string_find_string(test_str, 0, "abc", TRUE));
    EXPECT_EQ(3, t_string_find_string(test_str, 1, "abc", TRUE));
    EXPECT_EQ(20, t_string_find_string(test_str, 0, "ACB", FALSE));
    EXPECT_EQ(-1, t_string_find_string(test_str, 0, "jj", TRUE));
    EXPECT_EQ(-1, t_string_find_string(test_str, 0, 
                "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj", TRUE));

    EXPECT_EQ(0, t_string_find_string_reverse(test_str, 2, "abc", TRUE));
    EXPECT_EQ(3, t_string_find_string_reverse(test_str, 6, "abc", TRUE));
    EXPECT_EQ(20, t_string_find_string_reverse(test_str, strlen(test_str) - 1, "ACB", FALSE));
    EXPECT_EQ(-1, t_string_find_string_reverse(test_str, strlen(test_str) - 1, "jj", TRUE));
    EXPECT_EQ(-1, t_string_find_string_reverse(test_str, strlen(test_str) - 1, 
                "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj", TRUE));
}

TEST(TstringTest, Convert)
{
    int int_val;
    tbool bool_val;
    EXPECT_TRUE(t_string_to_int("123", &int_val));
    EXPECT_EQ(123, int_val);
    EXPECT_FALSE(t_string_to_int("123a", &int_val));
    EXPECT_FALSE(t_string_to_int("-", &int_val));
    EXPECT_TRUE(t_string_to_int("-12", &int_val));
    EXPECT_EQ(-12, int_val);

    EXPECT_TRUE(t_string_to_bool("TruE", &bool_val));
    EXPECT_EQ(TRUE, bool_val);
    EXPECT_TRUE(t_string_to_bool("true", &bool_val));
    EXPECT_EQ(TRUE, bool_val);
    EXPECT_FALSE(t_string_to_bool("true1", &bool_val));
    EXPECT_FALSE(t_string_to_bool("false1", &bool_val));
    EXPECT_TRUE(t_string_to_bool("FALSE", &bool_val));
    EXPECT_EQ(FALSE, bool_val);
    EXPECT_TRUE(t_string_to_bool("fALse", &bool_val));
    EXPECT_EQ(FALSE, bool_val);
}

TEST(TstringTest, Trimmed)
{
    char test_str1[] = "    ";
    char test_str2[] = "";
    char test_str3[] = "  asdf   ";
    char test_str4[] = "aaa\n";

    char out[20];
    t_string_trimmed(test_str1, out);
    EXPECT_STREQ("", out);
    t_string_trimmed_head(test_str1, out);
    EXPECT_STREQ("", out);
    t_string_trimmed_tail(test_str1, out);
    EXPECT_STREQ("", out);

    t_string_trimmed(test_str2, out);
    EXPECT_STREQ("", out);
    t_string_trimmed_head(test_str2, out);
    EXPECT_STREQ("", out);
    t_string_trimmed_tail(test_str2, out);
    EXPECT_STREQ("", out);

    t_string_trimmed(test_str3, out);
    EXPECT_STREQ("asdf", out);
    t_string_trimmed_head(test_str3, out);
    EXPECT_STREQ("asdf   ", out);
    t_string_trimmed_tail(test_str3, out);
    EXPECT_STREQ("  asdf", out);

    t_string_remove_linebreak(test_str4, out);
    EXPECT_STREQ("aaa", out);
    t_string_remove_linebreak(test_str3, out);
    EXPECT_STREQ("  asdf   ", out);
}

TEST(TstringTest, Get)
{
    char test_str[] = "asdfasdf\nsadfasdf\nasdf";
    char line[30];
    EXPECT_EQ(8, t_string_get_line(line, test_str, 29, 0));
    EXPECT_STREQ("asdfasdf", line);
    EXPECT_EQ(0, t_string_get_line(line, test_str, 29, 8));
    EXPECT_STREQ("", line);
    EXPECT_EQ(6, t_string_get_line(line, test_str, 29, 11));
    EXPECT_STREQ("dfasdf", line);
    EXPECT_EQ(4, t_string_get_line(line, test_str, 29, 18));
    EXPECT_STREQ("asdf", line);
    EXPECT_EQ(-1, t_string_get_line(line, test_str, 29, 22));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
