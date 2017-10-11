#include "gtest/gtest.h"
#include "../src/level.h"
#include "../include/tlog/tlog.h"


#ifdef T_ENABLE_ASSERT
TEST(LevelTest, Death)
{
    ASSERT_DEATH(log_level_convert(NULL), "");
}
#endif

TEST(LevelTest, Convert)
{
    EXPECT_EQ((TLOG_DEBUG | TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert("*"));

    EXPECT_EQ((TLOG_DEBUG | TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert("debug"));
    EXPECT_EQ((TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert(">debug"));
    EXPECT_EQ((TLOG_DEBUG) & LEVEL_MASK, log_level_convert("=debug"));

    EXPECT_EQ((TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert("info"));
    EXPECT_EQ((TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert(">info"));
    EXPECT_EQ((TLOG_INFO) & LEVEL_MASK, log_level_convert("=info"));

    EXPECT_EQ((TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert("notice"));
    EXPECT_EQ((TLOG_WARN | TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert(">notice"));
    EXPECT_EQ((TLOG_NOTICE) & LEVEL_MASK, log_level_convert("=notice"));

    EXPECT_EQ((TLOG_WARN | TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert("warn"));
    EXPECT_EQ((TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert(">warn"));
    EXPECT_EQ((TLOG_WARN) & LEVEL_MASK, log_level_convert("=warn"));

    EXPECT_EQ((TLOG_ERROR | TLOG_FATAL) & LEVEL_MASK, log_level_convert("error"));
    EXPECT_EQ((TLOG_FATAL) & LEVEL_MASK, log_level_convert(">error"));
    EXPECT_EQ((TLOG_ERROR) & LEVEL_MASK, log_level_convert("=error"));

    EXPECT_EQ((TLOG_FATAL) & LEVEL_MASK, log_level_convert("fatal"));
    EXPECT_EQ(0, log_level_convert(">fatal"));
    EXPECT_EQ((TLOG_FATAL) & LEVEL_MASK, log_level_convert("=fatal"));
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

