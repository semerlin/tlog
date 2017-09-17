set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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


endif (GTEST_FOUND)



