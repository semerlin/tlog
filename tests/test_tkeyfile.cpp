/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
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
    EXPECT_DEATH(t_keyfile_contains_group(NULL, NULL), "");
    EXPECT_DEATH(t_keyfile_contains_key(NULL, NULL, NULL), "");
    EXPECT_DEATH(t_keyfile_ref(NULL), "");
    EXPECT_DEATH(t_keyfile_unref(NULL), "");
}

#endif

static tint kv_cb(void *key, void *value, void *userdata)
{
    if (0 == strcmp((const char *)key, "flag2"))
    {
        EXPECT_STREQ("b", (char *)value);
    }
    else if (0 == strcmp((const char *)key, "flag3"))
    {
        EXPECT_STREQ("c", (char *)value);
    }
    else if (0 == strcmp((const char *)key, "flag4"))
    {
        EXPECT_STREQ("d", (char *)value);
    }
    else if (0 == strcmp((const char *)key, "flag5"))
    {
        EXPECT_STREQ("true", (char *)value);
    }
    else if (0 == strcmp((const char *)key, "flag6"))
    {
        EXPECT_STREQ("234", (char *)value);
    }
    else if (0 == strcmp((const char *)key, "flag1"))
    {
        EXPECT_STREQ("a", (char *)value);
    }
    else
    {
        return -1;
    }

    return 0;
}

static tint kv_failed_cb(void *key, void *value, void *userdata)
{
    if (0 == strcmp((const char *)key, "flag2"))
    {
        EXPECT_STREQ("b", (char *)value);
    }
    else if (0 == strcmp((const char *)key, "flag3"))
    {
        EXPECT_STREQ("c", (char *)value);
    }
    else if (0 == strcmp((const char *)key, "flag4"))
    {
        EXPECT_STREQ("d", (char *)value);
    }
    else if (0 == strcmp((const char *)key, "flag5"))
    {
        EXPECT_STREQ("true", (char *)value);
    }
    else if (0 == strcmp((const char *)key, "flag6"))
    {
        EXPECT_STREQ("234", (char *)value);
    }
    else
    {
        return -1;
    }

    return 0;
}



TEST(TkeyfileTest, Function)
{
    tkeyfile *keyfile = t_keyfile_new();
    ASSERT_NE((void *)0, keyfile);
    int ret = t_keyfile_load_from_file(keyfile, filename);
    ASSERT_EQ(0, ret);
    EXPECT_TRUE(t_keyfile_contains_group(keyfile, "test1"));
    EXPECT_TRUE(t_keyfile_contains_group(keyfile, "test2"));
    EXPECT_FALSE(t_keyfile_contains_group(keyfile, "test"));
    EXPECT_TRUE(t_keyfile_contains_key(keyfile, "test1", "flag3"));
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
    t_keyfile_get_string(keyfile, "test2", "flag5", out, "sss");
    EXPECT_STREQ("", out);


    char *groups[8] = {0};
    for (int i = 0; i < 8; ++i)
    {
        groups[i] = new char[16];
    }
    t_keyfile_group_names(keyfile, groups);

    EXPECT_STREQ("test1", groups[0]);
    EXPECT_STREQ("test2", groups[1]);

    char *keys[16] = {0};
    for (int i = 0; i < 8; ++i)
    {
        keys[i] = new char[16];
    }
    t_keyfile_key_names(keyfile, groups[0], keys);

    EXPECT_STREQ("flag1", keys[0]);
    EXPECT_STREQ("flag2", keys[1]);
    EXPECT_STREQ("flag3", keys[2]);
    EXPECT_STREQ("flag4", keys[3]);
    EXPECT_STREQ("flag5", keys[4]);
    EXPECT_STREQ("flag6", keys[5]);

    EXPECT_EQ(5, t_keyfile_group_count(keyfile));
    EXPECT_EQ(6, t_keyfile_key_count(keyfile, groups[0]));
    EXPECT_EQ(5, t_keyfile_key_count(keyfile, groups[1]));

    // foreach test
    EXPECT_EQ(0, t_keyfile_group_foreach(keyfile, groups[0], kv_cb, NULL));
    EXPECT_EQ(-1, t_keyfile_group_foreach(keyfile, groups[0], kv_failed_cb, NULL));

    for (int i = 0; i < 8; ++i)
    {
        delete []groups[i];
    }

    for (int i = 0; i < 16; ++i)
    {
        delete []keys[i];
    }

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
