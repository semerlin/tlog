#include "gtest/gtest.h"
#include "../include/tlog/tlog.h"

char filename[40] = {0};

TEST(TlogTest, Function)
{
    tlog_init(filename);
}


int main(int argc, char **argv)
{
    if (argc > 1)
    {
        strcpy(filename, argv[1]);
    }
 
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
