#ifndef SPACE_VIZ_GEOM_SPHERE_H
#define SPACE_VIZ_GEOM_SPHERE_H

#include "algebras/conformal.h"
#include "algebras/euclidean.h"
#include <cmath>

namespace space {
    namespace geom {
        template <class Vec>
        class Sphere {
            using T = typename Vec::ScalarValue;
            using C = algebras::Conformal<T, Vec::Algebra::Metric::EuclideanDim()>;
            using E = algebras::Euclidean<T, Vec::Algebra::Metric::EuclideanDim()>;
            using EVec = typename C::EVec;
            using Dual = typename C::Vec;

           public:
            Sphere(const Vec &center, T radius)
            : center_(center)
            , radius_(radius)
            {
            }

            Sphere(const Dual &dual)
            : center_(C::Round::CenterPoint(dual).template Cast<EVec>().values)
            , radius_(C::Round::Radius(dual))
            {
            }

            Vec Point(T u, T v) const
            {
                auto theta = T(u * 2.f * M_PI);
                auto phi = T(v * M_PI);
                return center() + Vec(radius() * std::cos(theta) * std::sin(phi),
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
