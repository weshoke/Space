cmake_minimum_required(VERSION 3.7.1)

# Added for all targets in this file
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/brigand)
SET(CMAKE_CXX_FLAGS "-std=c++14")

project (space)
add_executable(space ${CMAKE_CURRENT_SOURCE_DIR}/tools/Space/Space/main.cpp)
