find_path(MATLIBR_INCLUDE_DIR NAMES matrix.h PATHS /usr/local/include/)

# Set CMake such that it only finds static libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
find_library(MATLIBR_LIBRARY NAMES matlibr PATHS /usr/local/lib/)
# Reset CMake to find all libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES .so .a .lib .dll .dylib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(matlibr DEFAULT_MSG MATLIBR_LIBRARY MATLIBR_INCLUDE_DIR)

if(MATLIBR_FOUND)
    message(STATUS "Found matlibr.")
    set(MATLIBR_INCLUDE_DIRS ${MATLIBR_INCLUDE_DIR})
    set(MATLIBR_LIBRARIES ${MATLIBR_LIBRARY})
else()
    message(FATAL_ERROR "Could not find matlibr.")
endif()
