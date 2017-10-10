set(SRC_LIST
    tassert.c
    tlog.c
    tsysdeps.c
    tlist.c
    tslist.c
    tshareptr.c
    thlist.c
    thash_string.c
    tstring.c
    level.c
    format.c
    category.h)

add_library(tlog SHARED ${SRC_LIST})
    
