#ifndef SPACE_ALGEBRA_H
#define SPACE_ALGEBRA_H

#include "basis.h"
#include "basis/span.h"
#include "multivector.h"

namespace space {
    namespace detail {
        template <class Algebra, class BasisSpan>
        using MultivectorSpan =
            brigand::transform<BasisSpan,
                               brigand::bind<Multivector, brigand::pin<Algebra>, brigand::_1>>;
    }

    template <class Metric_, class ScalarValue_>
    struct Algebra {
        using Metric = Metric_;
        using ScalarValue = ScalarValue_;

        // List of multivectors for each grade
        using BasisSpan = basis::span::Span<Metric, brigand::uint16_t<Metric::Dim>>;
        using MultivectorSpan = detail::MultivectorSpan<Algebra, BasisSpan>;

        // Named generic multivectors
        using Scalar = brigand::front<MultivectorSpan>;
        using Vector = brigand::at_c<MultivectorSpan, 1>;
        using PseudoScalar = brigand::back<MultivectorSpan>;

        // Aliases
        using S = Scalar;
        using Vec = Vector;
        using Pss = PseudoScalar;
    };
}

#endif
