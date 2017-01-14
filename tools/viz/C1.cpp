#include "app/args.h"
#include "app/shader_io.h"
#include "common.h"
#include <bgfx/bgfx.h>

struct PosColorVertex {
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;

    static void init()
    {
        ms_decl.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    };

    static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl PosColorVertex::ms_decl;

static PosColorVertex s_cubeVertices[] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},
    {1.0f, 1.0f, 1.0f, 0xff0000ff},
    {-1.0f, -1.0f, 1.0f, 0xff00ff00},
    {1.0f, -1.0f, 1.0f, 0xff00ffff},
    {-1.0f, 1.0f, -1.0f, 0xffff0000},
    {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00},
    {1.0f, -1.0f, -1.0f, 0xffffffff},
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
        m_vbh = bgfx::createVertexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)),
            PosColorVertex::ms_decl);

        // Create static index buffer.
        m_ibh = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_cubeTriStrip, sizeof(s_cubeTriStrip)));

        // Create program from shaders.
        m_program = LoadProgram("vs_cubes", "fs_cubes");
    }

    virtual int shutdown() BX_OVERRIDE
    {
        // Cleanup.
        bgfx::destroyIndexBuffer(m_ibh);
        bgfx::destroyVertexBuffer(m_vbh);
        bgfx::destroyProgram(m_program);

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

            // Submit 11x11 cubes.
            m_timeOffset += 1;
            auto time = float(m_timeOffset) * 0.05f;
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
                    bgfx::setVertexBuffer(m_vbh);
                    bgfx::setIndexBuffer(m_ibh);

                    // Set render states.
                    bgfx::setState(0 | BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);

                    // Submit primitive for rendering to view 0.
                    bgfx::submit(0, m_program);
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

    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle m_ibh;
    bgfx::ProgramHandle m_program;
    int64_t m_timeOffset;
};

ENTRY_IMPLEMENT_MAIN(C1);
