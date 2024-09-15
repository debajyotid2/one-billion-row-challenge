# This file is licensed under the WTFPL version 2 -- you can see the full
# license over at http://www.wtfpl.net/txt/copying/
#
# - Try to find Criterion
#
# Once done this will define
#  CRITERION_FOUND - System has Criterion
#  CRITERION_INCLUDE_DIRS - The Criterion include directories
#  CRITERION_LIBRARIES - The libraries needed to use Criterion

find_package(PkgConfig)

find_path(CRITERION_INCLUDE_DIR criterion/criterion.h
          PATH_SUFFIXES criterion)

# Set CMake such that it only finds static libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
find_library(NANOMSG_LIBRARY NAMES nanomsg libnanomsg)
find_library(NANOPB_LIBRARY NAMES protobuf_nanopb_static libprotobuf_nanopb_static)
find_library(CRITERION_LIBRARY NAMES criterion libcriterion)
# Reset CMake to find all libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES .so .a .lib .dll .dylib)
find_library(GIT2_LIBRARY NAMES git2 libgit2)

include(FindPackageHandleStandardArgs)
# handle the QUIET and REQUIRED arguments and set CRITERION_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(criterion DEFAULT_MSG
                                  CRITERION_LIBRARY CRITERION_INCLUDE_DIR)

mark_as_advanced(CRITERION_INCLUDE_DIR CRITERION_LIBRARY)

if (CRITERION_FOUND)
    message(STATUS "Found criteron") 
    set(CRITERION_LIBRARIES ${CRITERION_LIBRARY} ${NANOPB_LIBRARY} ${NANOMSG_LIBRARY} ${GIT2_LIBRARY})
    set(CRITERION_INCLUDE_DIRS ${CRITERION_INCLUDE_DIR})
else()
    message(FATAL "Could not find criterion")
endif()
