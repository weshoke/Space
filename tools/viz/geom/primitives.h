#ifndef SPACE_VIZ_GEOM_PRIMITIVES_H
#define SPACE_VIZ_GEOM_PRIMITIVES_H

#include <algorithm>

// Vec3 PerpendicularComponent(const Vec3 &v, const Vec3 &normal)
// {
//     return v - normal * normal.dot(v);
// }
//
// Vec3 InterpolatePolar(const Vec3 &axis0, const Vec3 &axis1, float theta)
// {
//     return axis0 * std::cos(theta) + axis1 * std::sin(theta);
// }
//
// Vec3 PerpendicularAxis(const Vec3 &axis, const Vec3 &ref_dir)
// {
//     if (std::isnan(ref_dir(0)) ||
//         std::abs(std::abs(axis.dot(ref_dir)) - 1.f) < tolerance::MinAlignmentDelta ||
//         ref_dir.squaredNorm() < tolerance::MinSquaredDistance) {
//         // Permute components to generate a new reference direction
//         auto permutation = Vec3(axis(2), axis(0), axis(1));
//         if (IsApprox(
//                 (permutation - axis).squaredNorm(), 0.f, tolerance::MinSquaredDistance)) {
//             permutation[0] = -permutation[0];
//         }
//         return PerpendicularComponent(permutation, axis).normalized();
//     }
//     return PerpendicularComponent(ref_dir, axis).normalized();
// }

namespace space {
    namespace geom {
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
