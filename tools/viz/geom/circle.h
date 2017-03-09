#ifndef SPACE_GEOM_CIRCLE_H
#define SPACE_GEOM_CIRCLE_H

#include "algebras/conformal.h"
#include "algebras/euclidean.h"
#include <cmath>

namespace space {
    namespace geom {
        template <class Vec>
        class Circle {
           public:
            using T = typename Vec::ScalarValue;
            using C = algebras::Conformal<T, Vec::Algebra::Metric::EuclideanDim()>;
            using E = algebras::Euclidean<T, Vec::Algebra::Metric::EuclideanDim()>;
            using EVec = typename C::EVec;
            using MV = typename C::Circle;
            using Dir = typename E::PlanarDual;

           public:
            Circle(const Vec &center, T radius, const Dir &normal)
            : center_(center)
            , radius_(radius)
            , normal_(normal)
            {
            }

            Circle(const MV &mv)
            : center_(C::Round::Point(C::Round::Center(mv)).template Cast<EVec>().values)
            , radius_(C::Round::Radius(mv))
            , normal_(Dir(C::Round::Direction(mv).Dual().values).Normalized())
            {
            }

            const Vec &center() const { return center_; }
            T radius() const { return radius_; }
            const Dir &normal() const { return normal_; }
           private:
            Vec center_;
            T radius_;
            Dir normal_;
        };
    }
}

#endif
