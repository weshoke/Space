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

set(BX_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bx/include/compat/osx)
set(BGFX_INCLUDES
    ${BX_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty/khronos)
set(BGFX_ENTRY_INCLUDES
    ${BGFX_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/examples/common)

add_library(bx STATIC
    vendor/bx/src/commandline.cpp
    vendor/bx/src/crtimpl.cpp
    vendor/bx/src/debug.cpp
    vendor/bx/src/os.cpp
    vendor/bx/src/string.cpp)
target_include_directories(bx PUBLIC ${BX_INCLUDES})

add_library(bgfx STATIC
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
    vendor/bgfx/src/vertexdecl.cpp)
target_include_directories(bgfx PUBLIC ${BGFX_INCLUDES})
target_link_libraries(bgfx bx)

add_library(bgfx_entry OBJECT
    vendor/bgfx/examples/common/entry/cmd.cpp
    vendor/bgfx/examples/common/entry/dbg.cpp
    vendor/bgfx/examples/common/entry/entry.cpp
    vendor/bgfx/examples/common/entry/entry_osx.mm
    vendor/bgfx/examples/common/entry/input.cpp)
target_include_directories(bgfx_entry PUBLIC ${BGFX_ENTRY_INCLUDES})

# shaderc dependency projects
add_subdirectory(vendor/bgfx/3rdparty/glslang)
add_subdirectory(vendor/bgfx/3rdparty/glsl-optimizer)
add_library(fcpp STATIC
    vendor/bgfx/3rdparty/fcpp/cpp1.c
    vendor/bgfx/3rdparty/fcpp/cpp2.c
    vendor/bgfx/3rdparty/fcpp/cpp3.c
    vendor/bgfx/3rdparty/fcpp/cpp4.c
    vendor/bgfx/3rdparty/fcpp/cpp5.c
    vendor/bgfx/3rdparty/fcpp/cpp6.c)

add_executable(shaderc
    vendor/bgfx/tools/shaderc/shaderc_glsl.cpp
    vendor/bgfx/tools/shaderc/shaderc_hlsl.cpp
    vendor/bgfx/tools/shaderc/shaderc_pssl.cpp
    vendor/bgfx/tools/shaderc/shaderc_spirv.cpp
    vendor/bgfx/tools/shaderc/shaderc.cpp
)
target_include_directories(shaderc PUBLIC
    ${BGFX_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty/glsl-optimizer/src/glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty/glslang/glslang/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty/glslang/StandAlone
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty/glslang
${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/3rdparty/fcpp)
target_compile_options(shaderc PUBLIC ${DEBUG_COMPILER_OPTIONS})
target_link_libraries(shaderc
    ${COCOA_LIBRARY}
    bx
    bgfx
    glslang
    SPIRV
    SPVRemapper
    HLSL
    OGLCompiler
    OSDependent
    glsl_optimizer
    fcpp)

set(SHADERC_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders"
    CACHE PATH "Output dir for shaderc" )

set(SHADERC_INCLUDE_PATHS
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/examples/01-cubes"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/bgfx/src")

file(MAKE_DIRECTORY ${SHADERC_OUTPUT_DIR})
function(compile_shaders _name)
    # Get include flags
    set(SHADERC_INCLUDE_FLAGS "")
    foreach(_path ${SHADERC_INCLUDE_PATHS})
        list(APPEND SHADERC_INCLUDE_FLAGS -i)
        get_filename_component(PATH_ABS ${_path} ABSOLUTE)
        list(APPEND SHADERC_INCLUDE_FLAGS ${PATH_ABS})
    endforeach()

    # Build list of commands and output files
    set(TARGET_FILES "")
    foreach(_file ${ARGN})
        get_source_file_property(FILE_IS_VERTEX_SHADER ${_file} VERTEX_SHADER )
        get_source_file_property(FILE_VARYING_DEF ${_file} VARYING_DEF )
        get_filename_component(FILE_ABS ${_file} ABSOLUTE)
        set(FILE_TYPE_FLAG "f")
        if(${FILE_IS_VERTEX_SHADER})
            set(FILE_TYPE_FLAG "v")
        endif()

        set(FILE_VARYING_FLAGS "")
        if(FILE_VARYING_DEF)
            get_filename_component(FILE_VARYING_DEF_ABS ${FILE_VARYING_DEF} ABSOLUTE)
            list(APPEND FILE_VARYING_FLAGS --varyingdef)
            list(APPEND FILE_VARYING_FLAGS ${FILE_VARYING_DEF_ABS})
        endif()

        get_filename_component(FILE_NAME ${_file} NAME_WE)
        set(FILE_OUTPUT_NAME "${FILE_NAME}.bin")
        set(FILE_OUTPUT "${SHADERC_OUTPUT_DIR}/${FILE_OUTPUT_NAME}")

        add_custom_command(OUTPUT ${FILE_OUTPUT_NAME}
            COMMAND "${CMAKE_BINARY_DIR}/shaderc"
            ARGS
                -f ${FILE_ABS}
                -o ${FILE_OUTPUT}
                --type ${FILE_TYPE_FLAG}
                ${SHADERC_INCLUDE_FLAGS}
                --platform osx
                ${FILE_VARYING_FLAGS})
        list(APPEND TARGET_FILES ${FILE_OUTPUT_NAME})
    endforeach()

    # Add output files to named target
    add_custom_target(${_name} DEPENDS ${TARGET_FILES})
    set_target_properties(shaders PROPERTIES BINARY_DIR ${SHADERC_OUTPUT_DIR})
endfunction()

set_source_files_properties(
    tools/viz/shaders/source/vs_color.sc
    PROPERTIES
        VERTEX_SHADER TRUE
        VARYING_DEF tools/viz/shaders/source/varying.def.sc)
set_source_files_properties(
    tools/viz/shaders/source/fs_color.sc
    PROPERTIES
        VERTEX_SHADER FALSE
        VARYING_DEF tools/viz/shaders/source/varying.def.sc)

compile_shaders(shaders
    tools/viz/shaders/source/vs_color.sc
    tools/viz/shaders/source/fs_color.sc
)

add_executable(C1
    tools/viz/C1.cpp
    tools/viz/app/args.cpp
    tools/viz/app/shader_io.cpp
    $<TARGET_OBJECTS:bgfx_entry>)
target_compile_options(C1 PUBLIC ${DEBUG_COMPILER_OPTIONS})

target_include_directories(C1 PUBLIC
    ${BGFX_ENTRY_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/brigand)

target_link_libraries(C1
    ${COCOA_LIBRARY}
    ${OPENGL_LIBRARY}
    bx
    bgfx)


message(STATUS ${TESTING})

# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
