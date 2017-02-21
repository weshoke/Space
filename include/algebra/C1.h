#ifndef SPACE_ALGEBRA_C1_H
#define SPACE_ALGEBRA_C1_H

#include "algebra.h"
#include "basis.h"
#include "metric.h"
// #include "product.h"

namespace space {
    namespace algebra {
        template <class Value>
        struct C1 {
            using Xform = basis::xform::Conformal<1, 2>;
            using Metric = space::Metric<2, 1, Xform>;
            using Algebra = space::Algebra<Metric, Value>;
            using VectorBasis = brigand::at_c<typename Algebra::BasisSpan, 1>;

            // Multivectors
            using Scalar = typename Algebra::Scalar;
            using Vector = typename Algebra::Vec;
            using Point = Vector;
            using PseudoScalar = typename Algebra::PseudoScalar;

            // Aliases
            using S = Scalar;
            using Vec = Vector;
            using Pnt = Point;
            using Pss = PseudoScalar;
        };
    }
}

#endif
