find_path(YATPOOL_INCLUDE_DIR NAMES yatpool.h PATHS /usr/local/include/)

# Set CMake such that it only finds static libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
find_library(YATPOOL_LIBRARY NAMES yatpool PATHS /usr/local/lib/)
# Reset CMake to find all libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES .so .a .lib .dll .dylib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(yatpool DEFAULT_MSG YATPOOL_LIBRARY YATPOOL_INCLUDE_DIR)

if(YATPOOL_FOUND)
    message(STATUS "Found yatpool.")
    set(YATPOOL_INCLUDE_DIRS ${YATPOOL_INCLUDE_DIR})
    set(YATPOOL_LIBRARIES ${YATPOOL_LIBRARY})
else()
    message(FATAL_ERROR "Could not find yatpool.")
endif()
