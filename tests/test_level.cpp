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
    ASSERT_EQ(TLOG_DEBUG | TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL, log_level_convert("*"));

    ASSERT_EQ(TLOG_DEBUG | TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL, log_level_convert("debug"));
    ASSERT_EQ(TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL, log_level_convert(">debug"));
    ASSERT_EQ(TLOG_DEBUG, log_level_convert("=debug"));

    ASSERT_EQ(TLOG_INFO | TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL, log_level_convert("info"));
    ASSERT_EQ(TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL, log_level_convert(">info"));
    ASSERT_EQ(TLOG_INFO, log_level_convert("=info"));

    ASSERT_EQ(TLOG_NOTICE | TLOG_WARN | TLOG_ERROR | TLOG_FATAL, log_level_convert("notice"));
    ASSERT_EQ(TLOG_WARN | TLOG_ERROR | TLOG_FATAL, log_level_convert(">notice"));
    ASSERT_EQ(TLOG_NOTICE, log_level_convert("=notice"));

    ASSERT_EQ(TLOG_WARN | TLOG_ERROR | TLOG_FATAL, log_level_convert("warn"));
    ASSERT_EQ(TLOG_ERROR | TLOG_FATAL, log_level_convert(">warn"));
    ASSERT_EQ(TLOG_WARN, log_level_convert("=warn"));

    ASSERT_EQ(TLOG_ERROR | TLOG_FATAL, log_level_convert("error"));
    ASSERT_EQ(TLOG_FATAL, log_level_convert(">error"));
    ASSERT_EQ(TLOG_ERROR, log_level_convert("=error"));

    ASSERT_EQ(TLOG_FATAL, log_level_convert("fatal"));
    ASSERT_EQ(0, log_level_convert(">fatal"));
    ASSERT_EQ(TLOG_FATAL, log_level_convert("=fatal"));
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::FLAGS_gtest_death_test_style = "fast";
    return RUN_ALL_TESTS();
}

