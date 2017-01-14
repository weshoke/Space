cmake_minimum_required(VERSION 3.7.1)
set(CMAKE_VERBOSE_MAKEFILE on)
project(space)

set(DEBUG_COMPILER_OPTIONS "-std=c++1y")
set(RELEASE_COMPILER_OPTIONS "-std=c++1y" "-O3")

set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10")

add_executable(space
    tools/Space/Space/main.cpp)
target_compile_options(space PUBLIC ${DEBUG_COMPILER_OPTIONS})
target_include_directories(space PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(space PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/brigand)

add_subdirectory(test)

find_library(COCOA_LIBRARY Cocoa)
find_library(OPENGL_LIBRARY OpenGL)

add_executable(C1
    tools/viz/C1.cpp
    tools/viz/app/args.cpp
    tools/viz/app/shader_io.cpp

    vendor/bx/src/commandline.cpp
    vendor/bx/src/crtimpl.cpp
    vendor/bx/src/debug.cpp
    vendor/bx/src/os.cpp
    vendor/bx/src/string.cpp
    vendor/bgfx/src/bgfx.cpp

    vendor/bgfx/src/debug_renderdoc.cpp
    vendor/bgfx/src/glcontext_eagl.mm
    vendor/bgfx/src/glcontext_egl.cpp
    vendor/bgfx/src/glcontext_glx.cpp
    vendor/bgfx/src/glcontext_nsgl.mm
    vendor/bgfx/src/glcontext_ppapi.cpp
    vendor/bgfx/src/glcontext_wgl.cpp
    vendor/bgfx/src/hmd.cpp
    vendor/bgfx/src/hmd_openvr.cpp
    vendor/bgfx/src/hmd_ovr.cpp
    vendor/bgfx/src/image.cpp
    vendor/bgfx/src/renderer_d3d11.cpp
    vendor/bgfx/src/renderer_d3d12.cpp
    vendor/bgfx/src/renderer_d3d9.cpp
    vendor/bgfx/src/renderer_gl.cpp
    vendor/bgfx/src/renderer_gnm.cpp
    vendor/bgfx/src/renderer_mtl.mm
    vendor/bgfx/src/renderer_noop.cpp
    vendor/bgfx/src/renderer_vk.cpp
    vendor/bgfx/src/shader.cpp
    vendor/bgfx/src/shader_dx9bc.cpp
    vendor/bgfx/src/shader_dxbc.cpp
    vendor/bgfx/src/shader_spirv.cpp
    vendor/bgfx/src/topology.cpp
    vendor/bgfx/src/vertexdecl.cpp
    vendor/bgfx/examples/common/entry/cmd.cpp
    vendor/bgfx/examples/common/entry/dbg.cpp
    vendor/bgfx/examples/common/entry/entry.cpp
    vendor/bgfx/examples/common/entry/entry_osx.mm
    vendor/bgfx/examples/common/entry/input.cpp
)

target_compile_options(C1 PUBLIC ${DEBUG_COMPILER_OPTIONS})
target_include_directories(C1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(C1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/brigand)
target_include_directories(C1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bx/include)
target_include_directories(C1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bx/include/compat/osx)
target_include_directories(C1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/include)
target_include_directories(C1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/examples/common)
target_include_directories(C1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty)
target_include_directories(C1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty)
target_include_directories(C1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty/khronos)
target_include_directories(C1 PUBLIC ${OPENGL_LIBRARY})

# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

target_link_libraries(C1
        ${COCOA_LIBRARY}
        ${OPENGL_LIBRARY})
