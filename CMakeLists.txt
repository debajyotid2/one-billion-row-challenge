cmake_minimum_required(VERSION 3.27)

project(onebrc)

set(CMAKE_C_COMPILER gcc)
set(EXECUTABLE_NAME onebrc)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/*.c)

add_executable(${EXECUTABLE_NAME} main.c ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
