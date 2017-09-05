set(SRC_LIST
    tassert.c
    tlog.c
    tsysdeps.c
    tlist.c
    tslist.c
    tshareptr.c)

add_library(tlog SHARED ${SRC_LIST})
    
