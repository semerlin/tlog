/**
 * Copyright 2017, Huang Yang <elious.huang@gmail.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#include "gtest/gtest.h"
#include "../src/thlist.h"


struct num_list
{
    int num;
    thlist_node node;
};



#ifdef T_ENABLE_ASSERT
TEST(ThlistTest, Death)
{
    thlist_head head;
    thlist_node node;
    EXPECT_DEATH(t_hlist_insert(NULL, &node), "");
    EXPECT_DEATH(t_hlist_insert(&head, NULL), "");
    EXPECT_DEATH(t_hlist_remove(NULL), "");
}
#endif

TEST(ThlistTest, Init)
{
    thlist_head head;
    thlist_node node;
    t_hlist_init_head(&head);
    EXPECT_EQ(NULL, head.first);
    t_hlist_init_node(&node);
    EXPECT_EQ(NULL, node.pprev);
    EXPECT_EQ(NULL, node.next);
}

TEST(ThlistTest, AddAndRemove)
{
    thlist_head head;
    thlist_node node1, node2, node3, node4;
    t_hlist_init_head(&head);
    t_hlist_init_node(&node1);
    t_hlist_init_node(&node2);
    t_hlist_init_node(&node3);
    t_hlist_init_node(&node4);
    EXPECT_TRUE(t_hlist_is_empty(&head));

    //insert
    t_hlist_insert(&head, &node1);
    EXPECT_EQ(1, t_hlist_length(&head));
    t_hlist_insert(&head, &node2);
    t_hlist_insert(&head, &node3);
    EXPECT_EQ(3, t_hlist_length(&head));
    t_hlist_insert(&head, &node4);
    EXPECT_EQ(4, t_hlist_length(&head));

    //remove
    t_hlist_remove(&node1);
    EXPECT_EQ(3, t_hlist_length(&head));
    t_hlist_remove(&node3);
    EXPECT_EQ(2, t_hlist_length(&head));
    t_hlist_remove(&node4);
    EXPECT_EQ(1, t_hlist_length(&head));
    t_hlist_remove(&node2);
    EXPECT_TRUE(t_hlist_is_empty(&head));
}

TEST(ThlistTest, Foreach)
{
    thlist_head head;
    num_list node1, node2, node3, node4;
    t_hlist_init_head(&head);
    t_hlist_init_node(&node1.node);
    t_hlist_init_node(&node2.node);
    t_hlist_init_node(&node3.node);
    t_hlist_init_node(&node4.node);
    node1.num = 1;
    node2.num = 2;
    node3.num = 3;
    node4.num = 4;

    t_hlist_insert(&head, &node1.node);
    t_hlist_insert(&head, &node2.node);
    t_hlist_insert(&head, &node3.node);
    t_hlist_insert(&head, &node4.node);

    thlist_node *pos;
    int i = 4;
    t_hlist_foreach(pos, &head)
    {
        EXPECT_EQ(i, t_hlist_entry(pos, num_list, node)->num);
        --i;
    }
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

