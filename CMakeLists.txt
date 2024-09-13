cmake_minimum_required(VERSION 3.27)

project(onebrc)

set(CMAKE_C_COMPILER g++)
set(EXECUTABLE_NAME onebrc)
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})

set(CMAKE_PREFIX_PATH /usr/local/lib /opt/OpenBLAS/lib)
set(CMAKE_MODULE_PATH ${PROJECT_ROOT_DIR}/cmake)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/*.c)

add_executable(${EXECUTABLE_NAME} main.c ${SOURCES})

find_package(openblas REQUIRED)
find_package(matlibr REQUIRED)
find_package(yatpool REQUIRED)

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(${EXECUTABLE_NAME} PRIVATE 
    ${YATPOOL_INCLUDE_DIRS}
    ${MATLIBR_INCLUDE_DIRS}
    ${OPENBLAS_INCLUDE_DIRS}
)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    m 
    dl 
    ${YATPOOL_LIBRARIES}
    ${MATLIBR_LIBRARIES} 
    ${OPENBLAS_LIBRARIES}
)
