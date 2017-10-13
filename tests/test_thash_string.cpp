/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "gtest/gtest.h"
#include "../src/thash_string.h"

typedef struct
{
    int num;
    thash_string_node node;
}num_list;


using namespace std;

#ifdef T_ENABLE_ASSERT
TEST(Thash_stringTest, Death)
{
    thash_string *hash_string = t_hash_string_new();
    thash_string_node node;
    ASSERT_NE((void *)0, hash_string);
    EXPECT_DEATH(t_hash_string_insert(NULL, &node), "");
    EXPECT_DEATH(t_hash_string_insert(hash_string, NULL), "");
    EXPECT_DEATH(t_hash_string_remove(NULL, "aa"), "");
    EXPECT_DEATH(t_hash_string_remove(hash_string, NULL), "");
    EXPECT_DEATH(t_hash_string_remove_node(NULL), "");
    EXPECT_DEATH(t_hash_string_get(NULL, "aa"), "");
    EXPECT_DEATH(t_hash_string_get(hash_string, NULL), "");
    EXPECT_DEATH(t_hash_string_contain(NULL, "aa"), "");
    EXPECT_DEATH(t_hash_string_contain(hash_string, NULL), "");
    EXPECT_DEATH(t_hash_string_count(NULL), "");
    EXPECT_DEATH(t_hash_string_capacity(NULL), "");
    EXPECT_DEATH(t_hash_string_free(NULL, NULL), "");
}
#endif

TEST(Thash_stringTest, Insert)
{
    thash_string *hash_string = t_hash_string_new();
    thash_string_node node1, node2, node3;
    ASSERT_NE((void *)0, hash_string);
    t_hash_string_init_node(&node1, "abc");
    t_hash_string_init_node(&node2, "defhh");
    t_hash_string_init_node(&node3, "1234567");
    t_hash_string_insert(hash_string, &node1);
    t_hash_string_insert(hash_string, &node2);
    t_hash_string_insert(hash_string, &node3);
    EXPECT_EQ(0, strcmp(t_hash_string_get(hash_string, "abc")->key, "abc"));
    EXPECT_EQ(0, strcmp(t_hash_string_get(hash_string, "defhh")->key, "defhh"));
    EXPECT_EQ(0, strcmp(t_hash_string_get(hash_string, "1234567")->key, "1234567"));

    char *key[5] = {0};
    for (int i = 0; i < 5; ++i)
    {
        key[i] = new char[16];
    }
    t_hash_string_keys(hash_string, key);

    EXPECT_EQ(0, strcmp(key[0], "1234567"));
    EXPECT_EQ(0, strcmp(key[1], "abc"));
    EXPECT_EQ(0, strcmp(key[2], "defhh"));

    for (int i = 0; i < 5; ++i)
    {
        delete []key[i];
    }
}

TEST(Thash_stringTest, Remove)
{
    thash_string *hash_string = t_hash_string_new();
    thash_string_node node1, node2, node3, node4;
    ASSERT_NE((void *)0, hash_string);
    t_hash_string_init_node(&node1, "abc");
    t_hash_string_init_node(&node2, "def");
    t_hash_string_init_node(&node3, "123");
    t_hash_string_init_node(&node4, "123abc");
    t_hash_string_insert(hash_string, &node1);
    t_hash_string_insert(hash_string, &node2);
    t_hash_string_insert(hash_string, &node3);
    t_hash_string_insert(hash_string, &node4);
    EXPECT_EQ(0, strcmp(t_hash_string_get(hash_string, "abc")->key, "abc"));
    EXPECT_TRUE(t_hash_string_contain(hash_string, "abc"));
    t_hash_string_remove(hash_string, "abc");
    EXPECT_EQ((void *)0, t_hash_string_get(hash_string, "abc"));
    EXPECT_FALSE(t_hash_string_contain(hash_string, "abc"));
    t_hash_string_remove(hash_string, "123");
    EXPECT_EQ(0, strcmp(t_hash_string_get(hash_string, "123abc")->key, "123abc"));
    EXPECT_EQ((void *)0, t_hash_string_get(hash_string, "123"));
    t_hash_string_remove(hash_string, "123");
}


static void int_to_string(unsigned char data, char *str)
{
    memset(str, 0, 4);
    str[0] = 0x30 + (data / 100);
    str[1] = 0x30 + (data / 10 % 10);
    str[2] = 0x30 + (data % 10);
}

TEST(Thash_stringTest, Rehash)
{
    thash_string *hash_string = t_hash_string_new();
    ASSERT_NE((void *)0, hash_string);
    thash_string_node *node;

    char str_val[4] = {0, 0, 0, 0};
    for (int i = 0; i < 12; ++i)
    {
        node = new thash_string_node;
        node->key = new char[8];
        int_to_string(i, str_val);
        strcpy(node->key, str_val);
        t_hash_string_insert(hash_string, node);
    }
    

    EXPECT_EQ(12, t_hash_string_count(hash_string));
    EXPECT_EQ(10, t_hash_string_capacity(hash_string));

    node = new thash_string_node;
    node->key = new char[8];
    int_to_string(12, str_val);
    strcpy(node->key, str_val);
    hash_string = t_hash_string_insert(hash_string, node);

    EXPECT_EQ(13, t_hash_string_count(hash_string));
    EXPECT_EQ(20, t_hash_string_capacity(hash_string));
}



int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
