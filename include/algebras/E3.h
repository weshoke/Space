#ifndef SPACE_ALGEBRAS_E3_H
#define SPACE_ALGEBRAS_E3_H

#include "euclidean.h"
#include "ops/product.h"
#include <cmath>

namespace space {
    namespace algebras {
        template <class Value>
        struct E3 : public Euclidean<Value, 3> {
            using Base = Euclidean<Value, 3>;
            using Vec = typename Base::Vec;

            // E3 specific functions
            static auto CrossProduct(const Vec &v1, const Vec &v2) { return (v1 ^ v2).Dual(); }
            static auto AxisAngle(const Vec &axis, float theta)
            {
                return std::cos(theta) + axis.Dual() * std::sin(theta);
            }
        };
    }
}

#endif
