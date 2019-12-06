cmake_minimum_required(VERSION 3.15)
project(aoc19)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        sources/common.cpp
        sources/day01.cpp
        sources/day02.cpp
        sources/day03.cpp
        sources/day04.cpp
        sources/day05.cpp
        sources/intcode_vm.cpp
        sources/day06.cpp)

include_directories(sources)

add_executable(aoc19 ${SOURCE_FILES} main.cpp)

add_executable(aoc19-benchmarks ${SOURCE_FILES} benchmarks.cpp)

target_compile_definitions(aoc19-benchmarks PRIVATE RUN_BENCHMARKS)

# Copy inputs to app directory
add_custom_command(TARGET aoc19 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/inputs"
        $<TARGET_FILE_DIR:aoc19>)
add_custom_command(TARGET aoc19-benchmarks POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/inputs"
        $<TARGET_FILE_DIR:aoc19-benchmarks>)