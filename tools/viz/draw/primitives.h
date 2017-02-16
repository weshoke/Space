#ifndef SPACE_VIZ_DRAW_PRIMITIVES_H
#define SPACE_VIZ_DRAW_PRIMITIVES_H

#include "algebra/E2.h"
#include "algebra/E3.h"
#include "geom/circle.h"
#include "geom/line_segment.h"

namespace viz {
    namespace draw {
        using E2f = space::algebra::E2<float>;
        using E3f = space::algebra::E3<float>;
        using Vec2 = typename E2f::Vec;
        using Vec3 = typename E3f::Vec;
        using LineSegment = space::geom::LineSegment<Vec3>;
        using Circle = space::geom::Circle<Vec3>;

        template <class T>
        int32_t Size();
    }
}

#endif
