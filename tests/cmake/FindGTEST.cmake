find_path(GTEST_INCLUDE_DIR NAMES gtest PATHS /usr/local/include /usr/include)
find_library(GTEST_LIBRARY NAMES gtest PATHS /usr/lib /usr/local/lib)

if (GTEST_INCLUDE_DIR AND GTEST_LIBRARY)
    set (GTEST_FOUND TRUE)
endif (GTEST_INCLUDE_DIR AND GTEST_LIBRARY)

if (GTEST_FOUND)
    if (NOT GTEST_FIND_QUIETLY)
        message (STATUS "found gtest: ${GTEST_LIBRARY}")
    endif (NOT GTEST_FIND_QUIETLY)
else (GTEST_FOUND)
    if (NOT GTEST_FIND_QUIETLY)
        message (FATAL_ERROR "can't find gtest library")
    endif (NOT GTEST_FIND_QUIETLY)
endif (GTEST_FOUND)


