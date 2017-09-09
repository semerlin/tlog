set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package (GTEST)
if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIR})

    add_executable(test_assert test_assert.cpp)
    target_link_libraries(test_assert ${GTEST_LIBRARY} pthread)
endif (GTEST_FOUND)



