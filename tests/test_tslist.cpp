#include "gtest/gtest.h"
#include "../src/tslist.h"


/*struct _tlist
{
    tlist *prev;
    tlist *next;
};
*/
#ifdef T_ENABLE_ASSERT
TEST(TslistTest, Death)
{
    tslist head, node;
    EXPECT_DEATH(t_slist_init_head(NULL), "");
    EXPECT_DEATH(t_slist_init_node(NULL), "");
    EXPECT_DEATH(t_slist_prepend(NULL, &node), "");
    EXPECT_DEATH(t_slist_prepend(&head, NULL), "");
    EXPECT_DEATH(t_slist_append(&head, NULL), "");
    EXPECT_DEATH(t_slist_append(NULL, &node), "");
    EXPECT_DEATH(t_slist_remove(NULL, &node), "");
    EXPECT_DEATH(t_slist_remove(&head, NULL), "");
    EXPECT_DEATH(t_slist_is_empty(NULL), "");
    EXPECT_DEATH(t_slist_is_last(NULL, &node), "");
    EXPECT_DEATH(t_slist_is_first(NULL, &node), "");
    EXPECT_DEATH(t_slist_length(NULL), "");
    EXPECT_DEATH(t_slist_free(NULL, NULL), "");
}
#endif

TEST(TslistTest, Init)
{
    tslist head = {NULL};
    tslist node = {&head};
    t_slist_init_head(&head);
    EXPECT_EQ(&head, head.next);
    t_slist_init_node(&node);
    EXPECT_EQ(NULL, node.next);
}

TEST(TslistTest, Add)
{
    tslist head, node1, node2, node3;
    t_slist_init_head(&head);
    t_slist_init_node(&node1);
    t_slist_init_node(&node2);
    t_slist_init_node(&node3);
    t_slist_prepend(&head, &node1);
    EXPECT_EQ(&node1, head.next);
    EXPECT_EQ(&head, node1.next);
    t_slist_prepend(&head, &node2);
    EXPECT_EQ(&node2, head.next);
    EXPECT_EQ(&head, node1.next);
    EXPECT_EQ(&node1, node2.next);
    t_slist_append(&head, &node3);
    EXPECT_EQ(&node3, node1.next);
    EXPECT_EQ(&head, node3.next);
}

TEST(TslistTest, Remove)
{
    tslist head, node1, node2, node3;
    t_slist_init_head(&head);
    t_slist_init_node(&node1);
    t_slist_init_node(&node2);
    t_slist_init_node(&node3);
    t_slist_append(&head, &node1);
    t_slist_append(&head, &node2);
    t_slist_append(&head, &node3);
    t_slist_remove(&head, &node2);
    EXPECT_EQ(&node1, head.next);
    EXPECT_EQ(&node3, node1.next);
    EXPECT_EQ(&head, node3.next);
    EXPECT_EQ(NULL, node2.next);
}

TEST(TslistTest, Check)
{
    tslist head, node1, node2, node3;
    t_slist_init_head(&head);
    t_slist_init_node(&node1);
    t_slist_init_node(&node2);
    t_slist_init_node(&node3);
    EXPECT_TRUE(t_slist_is_empty(&head));
    EXPECT_FALSE(t_slist_is_last(&head, &node1));
    EXPECT_FALSE(t_slist_is_first(&head, &node1));
    EXPECT_EQ(0, t_slist_length(&head));
    t_slist_append(&head, &node1);
    EXPECT_EQ(1, t_slist_length(&head));
    t_slist_append(&head, &node2);
    t_slist_append(&head, &node3);
    EXPECT_EQ(3, t_slist_length(&head));
    t_slist_remove(&head, &node2);
    EXPECT_FALSE(t_slist_is_empty(&head));
    EXPECT_TRUE(t_slist_is_last(&head, &node3));
    EXPECT_TRUE(t_slist_is_first(&head, &node1));
    EXPECT_EQ(2, t_slist_length(&head));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}
