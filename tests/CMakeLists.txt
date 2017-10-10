set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package (GTEST)
if (GTEST_FOUND)
    include_directories(../src)
    include_directories(${GTEST_INCLUDE_DIR})

    set(LIB_LIST
        ${GTEST_LIBRARY}
        pthread)

    set(COMMON_SRC_LIST
        ../src/tassert.c
        ../src/tsysdeps.c)


    #test tassert
    add_executable(test_tassert test_tassert.cpp 
                               ${COMMON_SRC_LIST})
    target_link_libraries(test_tassert ${LIB_LIST})

    #test tshareptr
    add_executable(test_tshareptr test_tshareptr.cpp 
                                 ../src/tshareptr.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_tshareptr ${LIB_LIST})

    #test tlist
    add_executable(test_tlist test_tlist.cpp 
                                 ../src/tlist.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_tlist ${LIB_LIST})

    #test tslist
    add_executable(test_tslist test_tslist.cpp 
                                 ../src/tslist.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_tslist ${LIB_LIST})

    #test thlist
    add_executable(test_thlist test_thlist.cpp 
                                 ../src/thlist.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_thlist ${LIB_LIST})

    #test thash_string
    add_executable(test_thash_string test_thash_string.cpp 
                                 ../src/thlist.c
                                 ../src/thash_string.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_thash_string ${LIB_LIST})

    #test tstring
    add_executable(test_tstring test_tstring.cpp 
                                 ../src/tstring.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_tstring ${LIB_LIST})

    #test tkeyfile
    add_executable(test_tkeyfile test_tkeyfile.cpp 
                                 ../src/thlist.c
                                 ../src/tlist.c
                                 ../src/tshareptr.c
                                 ../src/tstring.c
                                 ../src/thash_string.c
                                 ../src/tkeyfile.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_tkeyfile ${LIB_LIST})

    #test tlog
    add_executable(test_tlog test_tlog.cpp 
                                 ../src/thlist.c
                                 ../src/tlist.c
                                 ../src/tslist.c
                                 ../src/tshareptr.c
                                 ../src/tstring.c
                                 ../src/thash_string.c
                                 ../src/tkeyfile.c
                                 ../src/level.c
                                 ../src/format.c
                                 ../src/tlog.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_tlog ${LIB_LIST})

    #test level
    add_executable(test_level test_level.cpp 
                                 ../src/level.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_level ${LIB_LIST})

    #test format 
    add_executable(test_format test_format.cpp 
                                 ../src/thlist.c
                                 ../src/tlist.c
                                 ../src/tshareptr.c
                                 ../src/tstring.c
                                 ../src/thash_string.c
                                 ../src/tkeyfile.c
                                 ../src/format.c
                                 ${COMMON_SRC_LIST})
    target_link_libraries(test_format ${LIB_LIST})


endif (GTEST_FOUND)



