cmake_minimum_required(VERSION 3.20)

include(compiler_base.cmake)

# ------------- Project -----------------
#--------------

project(util_algorithm)

# ------------- Main module
#--------------

FILE(GLOB_RECURSE header_files CONFIGURE_DEPENDS include/*.hpp include/*.h)
FILE(GLOB_RECURSE source_files CONFIGURE_DEPENDS src/*.cpp)

message(STATUS "${PROJECT_NAME} Configuration")
message(STATUS "---- Header Files:")
    foreach(header ${header_files})
        message(STATUS "----\t ${header}")
    endforeach()
message(STATUS "---- Source Files:")
    foreach(source ${source_files})
        message(STATUS "----\t ${source}")
    endforeach()

# Main Function
add_executable(${PROJECT_NAME} ${header_files} ${source_files} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

# ------------- Library linkage
#--------------

target_link_libraries(${PROJECT_NAME}
        # Outer library
        PUBLIC pthread
        PUBLIC atomic)


enable_testing()

FILE(GLOB_RECURSE test_structure_source_files CONFIGURE_DEPENDS test/structure/*.cpp)
add_executable(structure_test ${test_structure_source_files} ${header_files} ${source_files})
target_include_directories(structure_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(structure_test
        PRIVATE pthread
        PRIVATE atomic
        PRIVATE gtest
        PRIVATE gtest_main)
add_test(NAME structure_test COMMAND structure_test)

FILE(GLOB_RECURSE test_algorithm_source_files CONFIGURE_DEPENDS test/algorithm/*.cpp)
add_executable(algorithm_test ${test_algorithm_source_files} ${header_files} ${source_files})
target_include_directories(algorithm_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(algorithm_test
        PRIVATE pthread
        PRIVATE atomic
        PRIVATE gtest
        PRIVATE gtest_main)
add_test(NAME algorithm_test COMMAND algorithm_test)


# ------------- Benchmark
#--------------

add_subdirectory(benchmark)