find_path(OPENBLAS_INCLUDE_DIR NAMES cblas.h PATHS /opt/OpenBLAS/include/)

# Set CMake such that it only finds static libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
find_library(OPENBLAS_LIBRARY NAMES openblas PATHS /opt/OpenBLAS/lib/)
# Reset CMake to find all libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES .so .a .lib .dll .dylib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(openblas DEFAULT_MSG OPENBLAS_LIBRARY OPENBLAS_INCLUDE_DIR)

if(OPENBLAS_FOUND)
    message(STATUS "Found openblas.")
    set(OPENBLAS_INCLUDE_DIRS ${OPENBLAS_INCLUDE_DIR})
    set(OPENBLAS_LIBRARIES ${OPENBLAS_LIBRARY})
else()
    message(FATAL_ERROR "Could not find openblas.")
endif()
