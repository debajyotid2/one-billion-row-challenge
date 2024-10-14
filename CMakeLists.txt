cmake_minimum_required(VERSION 3.27)

project(onebrc)

set(CMAKE_C_COMPILER g++)
set(PROJECT_LIBRARY_NAME onebrc)
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})

set(CMAKE_PREFIX_PATH /usr/local/lib /opt/OpenBLAS/lib)
set(CMAKE_MODULE_PATH ${PROJECT_ROOT_DIR}/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

set(DEPENDENCIES openblas matlibr yatpool criterion)
set(EXECUTABLES create_measurements analyze_v1 analyze_v2 analyze_v3)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)

add_library(${PROJECT_LIBRARY_NAME} ${SOURCES})

foreach(dep ${DEPENDENCIES})
    find_package(${dep} REQUIRED)
endforeach()

foreach(exe ${EXECUTABLES})
    add_executable(${exe} ${exe}.c)
    target_include_directories(${exe} PUBLIC ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(${exe} PRIVATE
        ${PROJECT_LIBRARY_NAME}
        ${YATPOOL_LIBRARIES}
        ${MATLIBR_LIBRARIES}
        ${OPENBLAS_LIBRARIES}
    )
endforeach()

enable_testing()
add_subdirectory(tests)
