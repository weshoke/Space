#ifndef SPACE_VIZ_DRAW_DRAW_H
#define SPACE_VIZ_DRAW_DRAW_H

#include <array>

namespace viz {
    namespace draw {
        void ClearWindowWithDepth(std::array<uint32_t, 2> window_size, uint32_t color);
        std::array<float, 4> ColorComponents(uint32_t color);
    }
}

#endif
