find_path(TLOG_INCLUDE_DIR NAMES tlog PATHS /usr/local/include /usr/include)
find_library(TLOG_LIBRARY NAMES tlog PATHS /usr/lib /usr/local/lib)

if (TLOG_INCLUDE_DIR AND TLOG_LIBRARY)
    set (TLOG_FOUND TRUE)
endif (TLOG_INCLUDE_DIR AND TLOG_LIBRARY)

if (TLOG_FOUND)
    if (NOT TLOG_FIND_QUIETLY)
        message (STATUS "found tlog: ${TLOG_LIBRARY}")
    endif (NOT TLOG_FIND_QUIETLY)
else (TLOG_FOUND)
    if (NOT TLOG_FIND_QUIETLY)
        message (FATAL_ERROR "can't find tlog library")
    endif (NOT TLOG_FIND_QUIETLY)
endif (TLOG_FOUND)


