cmake_minimum_required(VERSION 2.8.12)

project (tlog)

option(TLOG_BUILD_EXAMPLES "Build the tlog examples programs" OFF)
option(TLOG_BUILD_TESTS "Build the tlog test programs" ON)

#-------------------------------------------------
# compiler flags 
#-------------------------------------------------
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC -fsigned-char")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsigned-char")
message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -g -ggdb -DT_ENABLE_ASSERT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -ggdb -DT_ENABLE_ASSERT")
    message(STATUS "Build Flags:" ${CMAKE_C_FLAGS_DEBUG})
else(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
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

