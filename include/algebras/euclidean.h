#ifndef SPACE_ALGEBRAS_EUCLIDEAN_H
#define SPACE_ALGEBRAS_EUCLIDEAN_H

#include "algebra.h"
#include "basis.h"
#include "metric.h"

namespace space {
    namespace algebras {
        template <class Value_, uint16_t EuclideanDim_>
        struct Euclidean {
            using Value = Value_;
            static constexpr auto EuclideanDim = EuclideanDim_;
            using Self = Euclidean<Value, EuclideanDim>;

            using Metric = space::Metric<EuclideanDim>;
            using Algebra = space::Algebra<Metric, Value>;
            using VectorBasis = brigand::at_c<typename Algebra::BasisSpan, 1>;
            using RotorBasis = ops::ProductBasisGp<Metric, VectorBasis, VectorBasis>;
            using PlanarDualBasis = brigand::at_c<typename Algebra::BasisSpan, EuclideanDim - 2>;

            // Multivectors
            using Scalar = typename Algebra::Scalar;
            using Vector = typename Algebra::Vec;
            using PseudoScalar = typename Algebra::PseudoScalar;
            using Rotor = Multivector<Algebra, RotorBasis>;
            using PlanarDual = Multivector<Algebra, PlanarDualBasis>;

            // Aliases
            using S = Scalar;
            using Vec = Vector;
            using Pss = PseudoScalar;
            using Rot = Rotor;
        };
    }
}

#endif
