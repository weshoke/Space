#ifndef SPACE_ALGEBRA_E3_H
#define SPACE_ALGEBRA_E3_H

#include "algebra.h"
#include "basis.h"
#include "metric.h"
#include "product.h"

namespace space {
    namespace algebra {
        template <class Value>
        struct E3 {
            using Metric = space::Metric<3>;
            using Algebra = space::Algebra<Metric, Value>;
            using VectorBasis = brigand::at_c<typename Algebra::BasisSpan, 1>;
            using QuaternionBasis =
                space::product::ProductBasisGp<Metric, VectorBasis, VectorBasis>;

            // Multivectors
            using Scalar = typename Algebra::Scalar;
            using Vector = typename Algebra::Vec;
            using PseudoScalar = typename Algebra::PseudoScalar;
            using Quaternion = Multivector<Algebra, QuaternionBasis>;

            // Aliases
            using S = Scalar;
            using Vec = Vector;
            using Quat = Quaternion;
            using Pss = PseudoScalar;
        };
    }
}

#endif
