cmake_minimum_required(VERSION 3.7.1)
set(CMAKE_VERBOSE_MAKEFILE on)
project(space)

set(DEBUG_COMPILER_OPTIONS "-std=c++1z" "-g" "-Wno-missing-braces" "-Wmissing-field-initializers")
set(RELEASE_COMPILER_OPTIONS "-std=c++1z" "-O3" "-Wno-missing-braces" "-Wmissing-field-initializers")

set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10")

add_executable(space
    tools/Space/Space/main.cpp)
target_compile_options(space PUBLIC ${DEBUG_COMPILER_OPTIONS})
target_include_directories(space PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(space PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/brigand)

add_subdirectory(test)

find_library(COCOA_LIBRARY Cocoa)
find_library(OPENGL_LIBRARY OpenGL)

add_subdirectory(vendor/glfw)

add_library(glad STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/deps/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/deps)

add_executable(C1
    tools/viz/C1.cpp
    tools/viz/dbg/type.cpp
    tools/viz/draw/color.cpp
    tools/viz/draw/draw.cpp
    tools/viz/draw/mesh.h
    tools/viz/draw/pipeline.h
    tools/viz/draw/primitives.cpp
    tools/viz/draw/renderable.cpp
    tools/viz/draw/mesh_factory.cpp
    tools/viz/draw/renderable_factory.cpp
    tools/viz/draw/renderable_factory.h
    tools/viz/gl/primitives.cpp
)
target_compile_options(C1 PUBLIC ${DEBUG_COMPILER_OPTIONS})

target_include_directories(C1 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/brigand
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/variant/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/filesystem
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/viz
)

target_link_libraries(C1
    ${COCOA_LIBRARY}
    ${OPENGL_LIBRARY}
    glfw
    glad
)


message(STATUS ${TESTING})

# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
