cmake_minimum_required(VERSION 2.8.12)

project (zlog)

option(TLOG_BUILD_EXAMPLES "Build the tlog examples programs" OFF)
option(TLOG_BUILD_TESTS "Build the tlog test programs" ON)

#-------------------------------------------------
# compiler flags 
#-------------------------------------------------
message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -Wall -g -ggdb -DT_ENABLE_ASSERT")
    message(STATUS "Build Flags:" ${CMAKE_C_FLAGS_DEBUG})
else(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -Wall -DNDEBUG")
    message(STATUS "Build Flags:" ${CMAKE_C_FLAGS_RELEASE})
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")


#-------------------------------------------------
# add main directory 
#-------------------------------------------------
add_subdirectory(src)

#-------------------------------------------------
# add subdiectories
#-------------------------------------------------
if(TLOG_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(TLOG_BUILD_TESTS)
    add_subdirectory(tests)   
endif()    

