#ifndef SPACE_ALGEBRA_H
#define SPACE_ALGEBRA_H

#include "basis.h"
#include "multivector.h"

namespace space {
    template <class Metric_, class ScalarValue_>
    struct Algebra {
        using Metric = Metric_;
        using ScalarValue = ScalarValue_;
        static constexpr auto Dim = Metric::Dim;

        template <class Basis>
        using Multivector = Multivector<Algebra, Basis>;

        using ScalarBasis = basis::Scalar<Algebra>;
        using VectorBasis = basis::Vectors<Algebra>;
        using PseudoScalarBasis = basis::PseudoScalar<Algebra>;

        using Scalar = Multivector<ScalarBasis>;
        using Vec = Multivector<VectorBasis>;
        using PseudoScalar = Multivector<PseudoScalarBasis>;

        // Aliases
        using S = Scalar;
        using Pss = PseudoScalar;
    };
}

#endif
