#include "draw.h"
#include "color.h"
#include "glad/glad.h"

namespace viz {
    namespace draw {

        void ClearWindowWithDepth(std::array<uint32_t, 2> window_size, uint32_t color)
        {
            glViewport(0, 0, window_size[0], window_size[1]);
            auto rgba = ColorComponents(color);
            glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
        }
    }
}
