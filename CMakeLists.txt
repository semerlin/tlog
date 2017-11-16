cmake_minimum_required(VERSION 2.8.12)

project (tlog)

set(VERSION_MAJOR    0)
set(VERSION_MINOR    6)
set(REVISION_NUMBER  0)
set(BUILD_NUMBER     0)

option(BUILD_SHARED "Build tlog shared library" OFF)
option(BUILD_TESTS "Build tlog unit tests programs" ON)

find_package(Threads REQUIRED)

#-------------------------------------------------
# compiler flags 
#-------------------------------------------------
set(COMMON_FLAGS "-Wall -Werror -fsigned-char")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -std=gnu99 -Wstrict-prototypes -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -g -ggdb -DT_ENABLE_ASSERT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -ggdb -DT_ENABLE_ASSERT")
    message(STATUS "C Flags:" ${CMAKE_C_FLAGS_DEBUG})
    message(STATUS "CXX Flags:" ${CMAKE_CXX_FLAGS_DEBUG})
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    message(STATUS "C Flags:" ${CMAKE_C_FLAGS_RELEASE})
    message(STATUS "CXX Flags:" ${CMAKE_CXX_FLAGS_RELEASE})
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

#-------------------------------------------------
# output path configure
#-------------------------------------------------
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#-------------------------------------------------
# add main directory 
#-------------------------------------------------
add_subdirectory(src)

#-------------------------------------------------
# add subdiectories
#-------------------------------------------------
if (BUILD_TESTS)
    add_subdirectory(tests)   
endif()    

#-------------------------------------------------
# install tlog
#-------------------------------------------------
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/tlog
    DESTINATION "include")



#examples use tlog library, so we can't build it here
#build it manual in examples folder after install tlog library
#option(TLOG_BUILD_EXAMPLES "Build the tlog examples programs" OFF)
#if(TLOG_BUILD_EXAMPLES)
#    add_subdirectory(examples)
#endif()




