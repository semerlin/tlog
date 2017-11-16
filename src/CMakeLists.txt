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
    set_target_properties(tlog PROPERTIES VERSION 
        ${VERSION_MAJOR}.${VERSION_MINOR}.${REVISION_NUMBER}.${BUILD_NUMBER} SOVERSION ${VERSION_MAJOR})
    install(TARGETS tlog
        LIBRARY DESTINATION "lib")
else (BUILD_SHARED)
    add_library(tlog STATIC ${SRC_LIST})
    install(TARGETS tlog
        ARCHIVE DESTINATION "lib")
endif (BUILD_SHARED)

#-------------------------------------------------
# generate and install pc
#-------------------------------------------------
configure_file(
    cmake/tlog.pc.in
    "${CMAKE_BINARY_DIR}/tlog.pc"
    @ONLY)

install(FILES "${CMAKE_BINARY_DIR}/tlog.pc" DESTINATION "/usr/lib/pkgconfig")


