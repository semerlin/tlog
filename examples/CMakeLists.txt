cmake_minimum_required(VERSION 2.8.12)

set(COMMON_FLAGS "-Wall -Werror -fsigned-char")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -O0 -g -ggdb -std=gnu99 -Wstrict-prototypes")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


find_package (TLOG)
if (TLOG_FOUND)
    include_directories(${TLOG_INCLUDE_DIR})

    #benchmark
    add_executable(benchmark benchmark.c)
    target_link_libraries(benchmark ${TLOG_LIBRARY})

endif (TLOG_FOUND)


