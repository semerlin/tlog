#include "tlog/tlog.h"

int main(int argc, char **argv)
{
    tlog_init((void *)0);
    const tlog_category *cat = tlog_get_category("*");
    tlog_debug(cat, "hello tlog");
    return 0;
}
