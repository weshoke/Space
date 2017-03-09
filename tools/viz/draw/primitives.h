#ifndef SPACE_VIZ_DRAW_PRIMITIVES_H
#define SPACE_VIZ_DRAW_PRIMITIVES_H

#include "algebras/C2.h"
#include "algebras/C3.h"
#include "algebras/E2.h"
#include "algebras/E3.h"
#include "geom/circle.h"
#include "geom/line.h"
#include "geom/line_segment.h"
#include "geom/matrix4.h"
#include "geom/sphere.h"

namespace viz {
    namespace draw {
        using E2 = space::algebras::E2<float>;
        using E3 = space::algebras::E3<float>;
        using C2 = space::algebras::C2<float>;
        using C3 = space::algebras::C3<float>;

        using Vec2 = typename E2::Vec;
        using Vec3 = typename E3::Vec;
        using Point2d = typename C2::Vec;
        using Point = typename C3::Vec;

        using LineSegment2d = space::geom::LineSegment<Vec2>;
        using LineSegment = space::geom::LineSegment<Vec3>;
        using Line2d = space::geom::Line<Vec2>;
        using Line = space::geom::Line<Vec3>;
        using Circle2d = space::geom::Circle<Vec2>;
        using Circle = space::geom::Circle<Vec3>;
        using Sphere = space::geom::Sphere<Vec3>;
        using Matrix4 = space::geom::Matrix4<float>;

        template <class T>
        int32_t Size();
    }
}

#endif
