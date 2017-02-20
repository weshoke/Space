#ifndef SPACE_VIZ_GEOM_PRIMITIVES_H
#define SPACE_VIZ_GEOM_PRIMITIVES_H

#include <algorithm>
#include <cmath>

namespace space {
    namespace geom {
        template <class T>
        T RadToDeg(T v)
        {
            return v * 180.f / M_PI;
        }

        template <class T>
        T DegToRad(T v)
        {
            return v * M_PI / 180.f;
        }

        // Rotate the coordinates of the input element
        template <class Vec>
        Vec RotateCoordinates(const Vec &v)
        {
            auto res = Vec(v);
            std::rotate(res.values.begin(), res.values.begin() + 1, res.values.end());
            return res;
        }

        // Returns the rejection of a from b
        template <class A, class B>
        auto Rejection(const A &a, const B &b)
        {
            // TODO: use exression templates to automatically derive that
            // the return type of this expression matches A without
            // having to cast when input types are the same
            return (a ^ b) / b;
        }

        // Returns an arbitrary axis perpendicular to the input axis
        template <class Vec>
        Vec PerpendicularAxis(const Vec &axis)
        {
            // assert(axis.Norm() == T{1})
            auto permutation = RotateCoordinates(axis);
            if ((permutation - axis).Norm() < 1e-5f) {
                permutation = -permutation;
            }
            return (Rejection(permutation, axis)).template Cast<Vec>().Normalized();
        }
    }
}

#endif
