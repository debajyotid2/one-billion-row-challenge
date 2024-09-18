cmake_minimum_required(VERSION 3.27)

project(onebrc)

set(CMAKE_C_COMPILER g++)
set(GENERATOR_EXECUTABLE_NAME create_measurements)
set(ANALYZER_EXECUTABLE_NAME analyze)
set(PROJECT_LIBRARY_NAME onebrc)
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})

set(CMAKE_PREFIX_PATH /usr/local/lib /opt/OpenBLAS/lib)
set(CMAKE_MODULE_PATH ${PROJECT_ROOT_DIR}/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)

add_executable(${GENERATOR_EXECUTABLE_NAME} create_measurements.c ${SOURCES})
add_executable(${ANALYZER_EXECUTABLE_NAME} analyze.c ${SOURCES})

add_library(${PROJECT_LIBRARY_NAME} ${SOURCES})

find_package(openblas REQUIRED)
find_package(matlibr REQUIRED)
find_package(yatpool REQUIRED)
find_package(criterion REQUIRED)

target_include_directories(${GENERATOR_EXECUTABLE_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(${GENERATOR_EXECUTABLE_NAME} PRIVATE 
    ${YATPOOL_INCLUDE_DIRS}
    ${MATLIBR_INCLUDE_DIRS}
    ${OPENBLAS_INCLUDE_DIRS}
)
target_link_libraries(${GENERATOR_EXECUTABLE_NAME} PRIVATE
    m 
    dl 
    ${YATPOOL_LIBRARIES}
    ${MATLIBR_LIBRARIES} 
    ${OPENBLAS_LIBRARIES}
)

target_include_directories(${ANALYZER_EXECUTABLE_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(${ANALYZER_EXECUTABLE_NAME} PRIVATE 
    ${YATPOOL_INCLUDE_DIRS}
    ${MATLIBR_INCLUDE_DIRS}
    ${OPENBLAS_INCLUDE_DIRS}
)
target_link_libraries(${ANALYZER_EXECUTABLE_NAME} PRIVATE
    m 
    dl 
    ${YATPOOL_LIBRARIES}
    ${MATLIBR_LIBRARIES} 
    ${OPENBLAS_LIBRARIES}
)

target_include_directories(${GENERATOR_EXECUTABLE_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(${GENERATOR_EXECUTABLE_NAME} PRIVATE 
    ${YATPOOL_INCLUDE_DIRS}
    ${MATLIBR_INCLUDE_DIRS}
    ${OPENBLAS_INCLUDE_DIRS}
)
target_link_libraries(${GENERATOR_EXECUTABLE_NAME} PRIVATE
    m 
    dl 
    ${YATPOOL_LIBRARIES}
    ${MATLIBR_LIBRARIES} 
    ${OPENBLAS_LIBRARIES}
)

enable_testing()
add_subdirectory(tests)
