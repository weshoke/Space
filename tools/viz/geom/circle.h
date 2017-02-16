#ifndef SPACE_VIZ_GEOM_CIRCLE_H
#define SPACE_VIZ_GEOM_CIRCLE_H

#include <cmath>

namespace space {
    namespace geom {
        template <class Vec>
        class Circle {
            using T = typename Vec::ScalarValue;

           public:
            Circle(const Vec &center, T radius, const Vec &normal)
            : center_(center)
            , radius_(radius)
            , normal_(normal)
            {
            }

            const Vec &center() const { return center_; }
            T radius() const { return radius_; }
            const Vec &normal() const { return normal_; }
           private:
            Vec center_;
            T radius_;
            Vec normal_;
        };
    }
}

#endif
