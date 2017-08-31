cmake_minimum_required(VERSION 2.8.12)

project (zlog)

message(STATUS "c flags = " ${CMAKE_C_FLAGS})

option(TLOG_BUILD_EXAMPLES "Build the tlog examples programs" OFF)
option(TLOG_BUILD_TESTS "Build the tlog test programs" OFF)


add_subdirectory(src)

#-------------------------------------------------
# add sundiectories
#-------------------------------------------------
if(TLOG_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(TLOG_BUILD_TESTS)
    add_subdirectory(tests)   
endif()    
