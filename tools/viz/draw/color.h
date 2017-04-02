#ifndef SPACE_VIZ_DRAW_COLOR_H
#define SPACE_VIZ_DRAW_COLOR_H

#include "primitives.h"
#include <algorithm>

namespace viz {
    namespace draw {
        namespace detail {
            constexpr uint32_t Convert(float v)
            {
                return uint32_t(std::min(std::max(0.f, v), 1.f) * 255.f);
            }

            constexpr float Convert(uint32_t v) { return float(v & 0xFF) / 255.f; }
        }

        constexpr uint32_t Color(float r, float g, float b, float a = 1.f)
        {
            return detail::Convert(r) << 24 | detail::Convert(g) << 16 | detail::Convert(b) << 8 |
                   detail::Convert(a);
        }

        uint32_t Color(Vec3 rgb, float a = 1.f);
        uint32_t Color(Vec4 rgba);

        constexpr uint32_t Color(float grey, float a = 1.f) { return Color(grey, grey, grey, a); }
        constexpr uint32_t Color(uint32_t rgb, float a = 1.f)
        {
            return rgb << 8 | detail::Convert(a);
        }

        Vec4 ColorComponents(uint32_t color);

        struct Colors {
            static const uint32_t orange;
            static const uint32_t yellow;
            static const uint32_t navy;
            static const uint32_t sky;
            static const uint32_t grass;
            static const uint32_t red;
            static const uint32_t cream;
            static const uint32_t grey;
            static const uint32_t black;
            static const uint32_t slate_grey;
            static const uint32_t indigo;
            static const uint32_t dark_violet;
            static const uint32_t orchid;
            static const uint32_t tomato;
            static const uint32_t porcelain;
            static const uint32_t plum;
            static const uint32_t light_cyan;
            static const uint32_t lemon_chiffon;
            static const uint32_t cadet_blue;
            static const uint32_t hot_pink;
            static const uint32_t white;
            static const uint32_t chromium;
        };
    }
}

#endif
