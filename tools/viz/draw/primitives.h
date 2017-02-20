#ifndef SPACE_VIZ_DRAW_PRIMITIVES_H
#define SPACE_VIZ_DRAW_PRIMITIVES_H

#include "algebra/E2.h"
#include "algebra/E3.h"
#include "geom/circle.h"
#include "geom/line_segment.h"
#include "geom/matrix4.h"

namespace viz {
    namespace draw {
        using E2 = space::algebra::E2<float>;
        using E3 = space::algebra::E3<float>;
        using Vec2 = typename E2::Vec;
        using Vec3 = typename E3::Vec;
        using LineSegment = space::geom::LineSegment<Vec3>;
        using Circle = space::geom::Circle<Vec3>;
        using Matrix4 = space::geom::Matrix4<float>;

        template <class T>
        int32_t Size();
    }
}

#endif