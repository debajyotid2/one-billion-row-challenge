# Configure testing
set(TEST_BINS onebrc.test)

file(GLOB_RECURSE TEST_SOURCES ${PROJECT_ROOT_DIR}/tests/*.c)

message(STATUS "Testing enabled. Building test binaries ...")
add_executable(${TEST_BINS} ${TEST_SOURCES})

target_include_directories(${TEST_BINS} PUBLIC ${PROJECT_ROOT_DIR}/src)
target_include_directories(${TEST_BINS} PRIVATE 
    ${YATPOOL_INCLUDE_DIRS}
    ${MATLIBR_INCLUDE_DIRS}
    ${OPENBLAS_INCLUDE_DIRS}
    ${CRITERION_INCLUDE_DIRS}
)
target_link_libraries(${TEST_BINS} PRIVATE
    m 
    dl
    ${PROJECT_LIBRARY_NAME}
    ${YATPOOL_LIBRARIES}
    ${MATLIBR_LIBRARIES} 
    ${OPENBLAS_LIBRARIES}
    ${CRITERION_LIBRARIES}
)

include(CTest)
add_test(Criterion_tests, "${CMAKE_CURRENT_BINARY_DIR}/${TEST_BINS}")
