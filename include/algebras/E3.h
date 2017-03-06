#ifndef SPACE_ALGEBRA_E3_H
#define SPACE_ALGEBRA_E3_H

#include "algebra.h"
#include "basis.h"
#include "metric.h"
#include "ops/product.h"
#include <cmath>

namespace space {
    namespace algebra {
        template <class Value>
        struct E3 {
            using Metric = space::Metric<3>;
            using Algebra = space::Algebra<Metric, Value>;
            using VectorBasis = brigand::at_c<typename Algebra::BasisSpan, 1>;
            using RotorBasis = ops::ProductBasisGp<Metric, VectorBasis, VectorBasis>;

            // Multivectors
            using Scalar = typename Algebra::Scalar;
            using Vector = typename Algebra::Vec;
            using PseudoScalar = typename Algebra::PseudoScalar;
            using Rotor = Multivector<Algebra, RotorBasis>;

            // Aliases
            using S = Scalar;
            using Vec = Vector;
            using Rot = Rotor;
            using Pss = PseudoScalar;

            // E3 specific functions
            static Vec CrossProduct(const Vec &v1, const Vec &v2) { return (v1 ^ v2).Dual(); }
            static Rot AxisAngle(const Vec &axis, float theta)
            {
                return std::cos(theta) + axis.Dual() * std::sin(theta);
            }
        };
    }
}

#endif
