#include "gtest/gtest.h"
#include "../src/thash_string.h"

#if 0
typedef struct _thash_string thash_string;

typedef struct 
{
    char *key;
    thlist_node node;
}thash_string_node;

T_EXTERN thash_string *t_hash_string_new(void);
T_EXTERN void t_hash_string_insert(thash_string *hash_string, thash_string_node *node);
T_EXTERN thash_string_node *t_hash_string_remove(thash_string *hash_string, const char *key);
T_EXTERN void t_hash_string_remove_node(thash_string_node *node);
T_EXTERN thash_string_node *t_hash_string_get(thash_string *hash_string, const char *key);
T_EXTERN tbool t_hash_string_contain(thash_string *hash_string, const char *key);
T_EXTERN void t_hash_string_free(thash_string *head, tfree_callback free_func);
#endif

typedef struct
{
    int num;
    thash_string_node node;
}num_list;



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
    EXPECT_DEATH(t_hash_string_free(NULL, NULL), "");
}
#endif

TEST(Thash_stringTest, Insert)
{
    thash_string *hash_string = t_hash_string_new();
    thash_string_node node1, node2, node3;
    ASSERT_NE((void *)0, hash_string);
    t_hash_string_init_node(&node1, "abc");
    t_hash_string_init_node(&node2, "def");
    t_hash_string_init_node(&node3, "123");
    t_hash_string_insert(hash_string, &node1);
    t_hash_string_insert(hash_string, &node2);
    t_hash_string_insert(hash_string, &node3);
    EXPECT_EQ(0, strcmp(t_hash_string_get(hash_string, "abc")->key, "abc"));
    EXPECT_EQ(0, strcmp(t_hash_string_get(hash_string, "def")->key, "def"));
    EXPECT_EQ(0, strcmp(t_hash_string_get(hash_string, "123")->key, "123"));
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



int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
