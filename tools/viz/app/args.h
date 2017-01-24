#ifndef SPACE_VIZ_APP_ARGS_H
#define SPACE_VIZ_APP_ARGS_H

#include <bgfx/bgfx.h>

struct Args {
    Args(int _argc, char** _argv);

    bgfx::RendererType::Enum m_type;
    uint16_t m_pciId;
};

#endif
