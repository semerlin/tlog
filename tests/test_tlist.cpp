#include "gtest/gtest.h"
#include "../src/tlist.h"


/*struct _tlist
{
    tlist *prev;
    tlist *next;
};
*/
#ifdef T_ENABLE_ASSERT
TEST(TlistTest, Death)
{
    tlist head, node;
    EXPECT_DEATH(t_list_init_head(NULL), "");
    EXPECT_DEATH(t_list_init_node(NULL), "");
    EXPECT_DEATH(t_list_prepend(NULL, &node), "");
    EXPECT_DEATH(t_list_prepend(&head, NULL), "");
    EXPECT_DEATH(t_list_append(&head, NULL), "");
    EXPECT_DEATH(t_list_append(NULL, &node), "");
    EXPECT_DEATH(t_list_remove(NULL), "");
    EXPECT_DEATH(t_list_is_empty(NULL), "");
    EXPECT_DEATH(t_list_is_last(NULL, &node), "");
    EXPECT_DEATH(t_list_is_first(NULL, &node), "");
    EXPECT_DEATH(t_list_length(NULL), "");
    EXPECT_DEATH(t_list_free(NULL, NULL), "");
}
#endif

TEST(TlistTest, Init)
{
    tlist head = {NULL, NULL};
    tlist node = {&head, &head};
    t_list_init_head(&head);
    EXPECT_EQ(&head, head.prev);
    EXPECT_EQ(&head, head.next);
    t_list_init_node(&node);
    EXPECT_EQ(NULL, node.prev);
    EXPECT_EQ(NULL, node.next);
}

TEST(TlistTest, Add)
{
    tlist head, node1, node2, node3;
    t_list_init_head(&head);
    t_list_init_node(&node1);
    t_list_init_node(&node2);
    t_list_init_node(&node3);
    t_list_prepend(&head, &node1);
    EXPECT_EQ(&node1, head.next);
    EXPECT_EQ(&node1, head.prev);
    EXPECT_EQ(&head, node1.next);
    EXPECT_EQ(&head, node1.prev);
    t_list_prepend(&head, &node2);
    EXPECT_EQ(&node2, head.next);
    EXPECT_EQ(&node1, head.prev);
    EXPECT_EQ(&head, node1.next);
    EXPECT_EQ(&node2, node1.prev);
    EXPECT_EQ(&head, node2.prev);
    EXPECT_EQ(&node1, node2.next);
    t_list_append(&head, &node3);
    EXPECT_EQ(&node3, node1.next);
    EXPECT_EQ(&node1, node3.prev);
    EXPECT_EQ(&head, node3.next);
}

TEST(TlistTest, Remove)
{
    tlist head, node1, node2, node3;
    t_list_init_head(&head);
    t_list_init_node(&node1);
    t_list_init_node(&node2);
    t_list_init_node(&node3);
    t_list_append(&head, &node1);
    t_list_append(&head, &node2);
    t_list_append(&head, &node3);
    t_list_remove(&node2);
    EXPECT_EQ(&node1, head.next);
    EXPECT_EQ(&node3, head.prev);
    EXPECT_EQ(&node3, node1.next);
    EXPECT_EQ(&head, node1.prev);
    EXPECT_EQ(&head, node3.next);
    EXPECT_EQ(&node1, node3.prev);
    EXPECT_EQ(NULL, node2.next);
    EXPECT_EQ(NULL, node2.prev);
}

TEST(TlistTest, Check)
{
    tlist head, node1, node2, node3;
    t_list_init_head(&head);
    t_list_init_node(&node1);
    t_list_init_node(&node2);
    t_list_init_node(&node3);
    EXPECT_TRUE(t_list_is_empty(&head));
    EXPECT_FALSE(t_list_is_last(&head, &node1));
    EXPECT_FALSE(t_list_is_first(&head, &node1));
    EXPECT_EQ(0, t_list_length(&head));
    t_list_append(&head, &node1);
    EXPECT_EQ(1, t_list_length(&head));
    t_list_append(&head, &node2);
    t_list_append(&head, &node3);
    EXPECT_EQ(3, t_list_length(&head));
    t_list_remove(&node2);
    EXPECT_FALSE(t_list_is_empty(&head));
    EXPECT_TRUE(t_list_is_last(&head, &node3));
    EXPECT_TRUE(t_list_is_first(&head, &node1));
    EXPECT_EQ(2, t_list_length(&head));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

