#ifndef SPACE_ALGEBRAS_E2_H
#define SPACE_ALGEBRAS_E2_H

#include "algebra.h"
#include "basis.h"
#include "metric.h"

namespace space {
    namespace algebras {
        template <class Value>
        struct E2 {
            using Metric = space::Metric<2>;
            using Algebra = space::Algebra<Metric, Value>;
            using VectorBasis = brigand::at_c<typename Algebra::BasisSpan, 1>;

            // Multivectors
            using Scalar = typename Algebra::Scalar;
            using Vector = typename Algebra::Vec;
            using PseudoScalar = typename Algebra::PseudoScalar;

            // Aliases
            using S = Scalar;
            using Vec = Vector;
            using Pss = PseudoScalar;
        };
    }
}

#endif
