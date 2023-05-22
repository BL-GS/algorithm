cmake_minimum_required(VERSION 3.20)

# ------------- Project -----------------
#--------------

project(util_algorithm_benchmark)

# ------------- Main module
#--------------

FILE(GLOB_RECURSE header_files CONFIGURE_DEPENDS ../include/*.hpp ../include/*.h)
FILE(GLOB_RECURSE source_files CONFIGURE_DEPENDS ../src/*.cpp)

message(STATUS "${PROJECT_NAME} Configuration")
message(STATUS "---- Header Files:")
foreach(header ${header_files})
    message(STATUS "----\t ${header}")
endforeach()
message(STATUS "---- Source Files:")
    foreach(source ${source_files})
        message(STATUS "----\t ${source}")
    endforeach()

FILE(GLOB_RECURSE strut_benchmark_file CONFIGURE_DEPENDS structure/*.cpp)
foreach(source ${strut_benchmark_file})
    GET_FILENAME_COMPONENT(source_bench ${source} NAME_WLE)

    message(STATUS "Benchmark\t ${source_bench}")
    # Main Function
    add_executable(${source_bench} ${header_files} ${source_files} ${source})

    target_include_directories(${source_bench} PUBLIC ../include)
    # ------------- Library linkage
    #--------------
    target_link_libraries(${source_bench}
            # Outer library
            PUBLIC pthread
            PUBLIC atomic
            PRIVATE benchmark)
endforeach()
