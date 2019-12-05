cmake_minimum_required(VERSION 3.15)
project(aoc19)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        main.cpp
        sources/common.cpp
        sources/day01.cpp
        sources/day02.cpp
        sources/day05.cpp sources/intcode_vm.cpp)

include_directories(sources)

add_executable(aoc19 ${SOURCE_FILES})

# Copy inputs to app dir
add_custom_command(TARGET aoc19 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/inputs"
        $<TARGET_FILE_DIR:aoc19>)