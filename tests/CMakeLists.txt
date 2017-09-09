set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package (GTEST)

if (GTEST_FOUND)
    add_executable(test_assert test_assert.cpp)
    target_link_libraries(test_assert gtest pthread)
endif (GTEST_FOUND)



