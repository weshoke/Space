cmake_minimum_required(VERSION 3.7.1)
project(space)

set(CMAKE_VERBOSE_MAKEFILE on)

set(DEBUG_COMPILER_OPTIONS "-std=c++1y")
set(RELEASE_COMPILER_OPTIONS "-std=c++1y" "-O3")

add_executable(space
    tools/Space/Space/main.cpp)
target_compile_options(space PUBLIC ${DEBUG_COMPILER_OPTIONS})
target_include_directories(space PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(space PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/brigand)

add_subdirectory(test)
