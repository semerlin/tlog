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
    tkeyfile.c
    mdc.c
    level.c
    format.c
    category.c
    rules.c)

add_library(tlog SHARED ${SRC_LIST})

#-------------------------------------------------
# install tlog
#-------------------------------------------------
install(TARGETS tlog
    LIBRARY DESTINATION "lib")


    
