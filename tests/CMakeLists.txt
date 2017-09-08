include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gtest/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/gtest/lib)

add_executable(test_assert test_assert.cpp)

target_link_libraries(test_assert gtest gtest_main pthread)
