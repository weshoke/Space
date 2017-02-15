#include "draw.h"

namespace viz {
    namespace draw {
        std::array<float, 4> ColorComponents(uint32_t color)
        {
            return {
                ((color & 0xFF000000) >> 24) / 255.f,
                ((color & 0x00FF0000) >> 16) / 255.f,
                ((color & 0x0000FF00) >> 8) / 255.f,
                ((color & 0x000000FF)) / 255.f,
            };
        }
    }
}
