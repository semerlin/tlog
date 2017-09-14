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

endif (GTEST_FOUND)



