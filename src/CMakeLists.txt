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

#-------------------------------------------------
# build and install tlog
#-------------------------------------------------
if (BUILD_SHARED)
    add_library(tlog SHARED ${SRC_LIST})
    install(TARGETS tlog
        LIBRARY DESTINATION "lib")
else (BUILD_SHARED)
    add_library(tlog STATIC ${SRC_LIST})
    install(TARGETS tlog
        ARCHIVE DESTINATION "lib")
endif (BUILD_SHARED)


