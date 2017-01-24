#include "app/args.h"
#include "app/shader_io.h"
#include "common.h"
#include <bgfx/bgfx.h>
#include <array>

struct PosColorVertex {
    float x;
    float y;
    float z;

    static void init()
    {
        ms_decl.begin().add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float).end();
    };

    static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl PosColorVertex::ms_decl;

static PosColorVertex s_cubeVertices[] = {
    {-1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
};

static const uint16_t s_cubeTriList[] = {
    0, 1, 2,           // 0
    1, 3, 2, 4, 6, 5,  // 2
    5, 6, 7, 0, 2, 4,  // 4
    4, 2, 6, 1, 5, 3,  // 6
    5, 7, 3, 0, 4, 1,  // 8
    4, 5, 1, 2, 3, 6,  // 10
    6, 3, 7,
};

static const uint16_t s_cubeTriStrip[] = {
    0, 1, 2, 3, 7, 1, 5, 0, 4, 2, 6, 7, 4, 5,
};

class C1 : public entry::AppI {
    void init(int _argc, char** _argv) BX_OVERRIDE
    {
        Args args(_argc, _argv);

        width_ = 1280;
        height_ = 720;
        debug_ = BGFX_DEBUG_TEXT;
        reset_ = BGFX_RESET_VSYNC;

        bgfx::init(args.m_type, args.m_pciId);
        bgfx::reset(width_, height_, reset_);

        // Enable debug text.
        bgfx::setDebug(debug_);

        // Set view 0 clear state.
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

        // Create vertex stream declaration.
        PosColorVertex::init();

        // Create static vertex buffer.
        vbh_ = bgfx::createVertexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
            PosColorVertex::ms_decl);

        // Create static index buffer.
        ibh_ = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeTriStrip, sizeof(s_cubeTriStrip)));

        // Create program from shaders.
        program_ = LoadProgram("vs_color", "fs_color");
        color_ = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);

        time_offset_ = 0;
    }

    virtual int shutdown() BX_OVERRIDE
    {
        // Cleanup.
        bgfx::destroyIndexBuffer(ibh_);
        bgfx::destroyVertexBuffer(vbh_);
        bgfx::destroyProgram(program_);
        bgfx::destroyUniform(color_);

        // Shutdown bgfx.
        bgfx::shutdown();
        return 0;
    }

    bool update() BX_OVERRIDE
    {
        if (!entry::processEvents(width_, height_, debug_, reset_)) {
            // Set view 0 default viewport.
            bgfx::setViewRect(0, 0, 0, uint16_t(width_), uint16_t(height_));

            // This dummy draw call is here to make sure that view 0 is cleared
            // if no other draw calls are submitted to view 0.
            bgfx::touch(0);

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf(0, 1, 0x4f, "bgfx/examples/01-cube");
            bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Rendering simple static mesh.");
            // bgfx::dbgTextPrintf(0, 3, 0x0f, "Frame: % 7.3f[ms]", double(frameTime)*toMs);

            float at[3] = {0.0f, 0.0f, 0.0f};
            float eye[3] = {0.0f, 0.0f, -35.0f};

            float view[16];
            bx::mtxLookAt(view, eye, at);

            float proj[16];
            bx::mtxProj(proj, 60.0f, float(width_) / float(height_), 0.1f, 100.0f);
            bgfx::setViewTransform(0, view, proj);

            // Set view 0 default viewport.
            bgfx::setViewRect(0, 0, 0, uint16_t(width_), uint16_t(height_));

            auto color = std::array<float, 4>{1.f, 1.f, 1.f, 1.f};
            bgfx::setUniform(color_, color.data());

            // Submit 11x11 cubes.
            time_offset_ += 1;
            auto time = float(time_offset_) * 0.05f;
            for (uint32_t yy = 0; yy < 11; ++yy) {
                for (uint32_t xx = 0; xx < 11; ++xx) {
                    float mtx[16];
                    bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
                    mtx[12] = -15.0f + float(xx) * 3.0f;
                    mtx[13] = -15.0f + float(yy) * 3.0f;
                    mtx[14] = 0.0f;

                    // Set model matrix for rendering.
                    bgfx::setTransform(mtx);

                    // Set vertex and index buffer.
                    bgfx::setVertexBuffer(vbh_);
                    bgfx::setIndexBuffer(ibh_);

                    // Set render states.
                    bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);

                    // Submit primitive for rendering to view 0.
                    bgfx::submit(0, program_);
                }
            }

            // Advance to next frame. Rendering thread will be kicked to
            // process submitted rendering primitives.
            bgfx::frame();

            return true;
        }

        return false;
    }

    uint32_t width_;
    uint32_t height_;
    uint32_t debug_;
    uint32_t reset_;

    bgfx::VertexBufferHandle vbh_;
    bgfx::IndexBufferHandle ibh_;
    bgfx::ProgramHandle program_;
    bgfx::UniformHandle color_;
    int64_t time_offset_;
};

ENTRY_IMPLEMENT_MAIN(C1);
