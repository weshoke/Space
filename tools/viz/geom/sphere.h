#ifndef SPACE_VIZ_GEOM_SPHERE_H
#define SPACE_VIZ_GEOM_SPHERE_H

#include <cmath>

namespace space {
    namespace geom {
        template <class Vec>
        class Sphere {
            using T = typename Vec::ScalarValue;

           public:
            Sphere(const Vec &center, T radius)
            : center_(center)
            , radius_(radius)
            {
            }

            Vec Point(T u, T v) const
            {
                auto theta = T(u * 2.f * M_PI);
                auto phi = T(v * M_PI);
                return Vec(radius() * std::cos(theta) * std::sin(phi),
                           radius() * std::sin(theta) * std::sin(phi),
                           radius() * std::cos(phi));
            }

            const Vec &center() const { return center_; }
            T radius() const { return radius_; }
           private:
            Vec center_;
            T radius_;
        };
    }
}

#endif
