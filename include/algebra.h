#ifndef SPACE_ALGEBRA_H
#define SPACE_ALGEBRA_H

#include "basis.h"
#include "multivector.h"
#include "product.h"

namespace space {
    namespace detail {
        template <class Algebra, uint16_t Dim>
        struct DimBasis {
            // TODO: some redundant code with Product
            static_assert(Algebra::Dim >= Dim, "Dimension Error");
            using VectorBasis = typename DimBasis<Algebra, 1>::type;
            using ChildDimBasis = typename DimBasis<Algebra, Dim - 1>::type;
            using Op = product::op::Op<brigand::_1>;
            using ProductLists =
                typename product::detail::BitProduct<ChildDimBasis, VectorBasis, Op>::type;
            using ProductBasis = brigand::transform<
                ProductLists,
                brigand::bind<
                    brigand::type_from,
                    brigand::bind<blade::Blade, brigand::bind<brigand::front, brigand::_1>>>>;
            using type = ProductBasis;
        };

        template <class Algebra>
        struct DimBasis<Algebra, 0> {
            using type = basis::Scalar<Algebra>;
        };

        template <class Algebra>
        struct DimBasis<Algebra, 1> {
            using type = basis::Vectors<Algebra>;
        };

        template <class Algebra, class Dim>
        using DimBasis_ = DimBasis<Algebra, Dim::value>;

        template <class Algebra>
        struct BasisVectors {
            using Dims = brigand::make_sequence<brigand::uint16_t<0>, Algebra::Dim + 1>;
            using type = brigand::transform<
                Dims,
                brigand::bind<brigand::type_from,
                              brigand::bind<DimBasis_, brigand::pin<Algebra>, brigand::_1>>>;
        };
    }

    template <class Metric_, class ScalarValue_>
    struct Algebra {
        using Metric = Metric_;
        using ScalarValue = ScalarValue_;
        static constexpr auto Dim = Metric::Dim;

        // TODO: these should be in Metric
        using BasisVectors = typename detail::BasisVectors<Algebra>::type;
        using ScalarBasis = basis::Scalar<Algebra>;
        using VectorBasis = basis::Vectors<Algebra>;
        using PseudoScalarBasis = basis::PseudoScalar<Algebra>;

        template <class Basis>
        using Multivector = Multivector<Algebra, Basis>;

        using Scalar = Multivector<ScalarBasis>;
        using Vec = Multivector<VectorBasis>;
        using PseudoScalar = Multivector<PseudoScalarBasis>;

        // Aliases
        using S = Scalar;
        using Pss = PseudoScalar;
    };
}

#endif
