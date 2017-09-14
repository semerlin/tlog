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


    #test assert
    add_executable(test_assert test_assert.cpp 
                               ${COMMON_SRC_LIST})
    target_link_libraries(test_assert ${LIB_LIST})

    #test shareptr
endif (GTEST_FOUND)



