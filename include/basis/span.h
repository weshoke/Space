#ifndef SPACE_BASIS_SPAN_H
#define SPACE_BASIS_SPAN_H

#include "brigand/algorithms/transform.hpp"
#include "brigand/functions/bitwise/shift_left.hpp"
#include "brigand/sequences/make_sequence.hpp"
#include "metric/diagonal.h"
#include "product.h"

namespace space {
    namespace basis {
        namespace span {
            namespace detail {
                using Scalar = brigand::list<brigand::uint16_t<0>>;

                template <class Metric>
                struct VectorList {
                    using type = brigand::transform<
                        brigand::make_sequence<brigand::uint16_t<0>, Metric::Dim>,
                        brigand::shift_left<brigand::uint16_t<1>, brigand::_1>>;
                };

                template <class Metric, uint16_t Dim>
                struct BasisSpan {
                    // TODO: some redundant code with Product
                    static_assert(Metric::Dim >= Dim, "Dimension Error");
                    using VectorBasis = typename BasisSpan<Metric, 1>::type;
                    using PrevBasis = typename BasisSpan<Metric, Dim - 1>::type;
                    using Op = product::op::Op<brigand::_1>;
                    using ProductLists = product::BitProduct<Metric, PrevBasis, VectorBasis, Op>;
                    using ProductBasis = brigand::transform<
                        ProductLists,
                        brigand::bind<blade::Blade, brigand::bind<brigand::front, brigand::_1>>>;
                    using type = ProductBasis;
                };

                template <class Metric>
                struct BasisSpan<Metric, 0> {
                    using type = Scalar;
                };

                template <class Metric>
                struct BasisSpan<Metric, 1> {
                    using type = typename VectorList<Metric>::type;
                };

                template <class Metric, class Dim>
                using BasisSpan_ = BasisSpan<Metric, Dim::value>;

                template <class Metric, class Dim>
                struct Span {
                    using DiagonalMetric = metric::Diagonal<Metric>;
                    using Dims = brigand::make_sequence<brigand::uint16_t<0>, Dim::value + 1>;
                    using type = brigand::transform<
                        Dims,
                        brigand::bind<
                            brigand::type_from,
                            brigand::bind<BasisSpan_, brigand::pin<DiagonalMetric>, brigand::_1>>>;
                };
            }

            template <class Metric, class Dim>
            using Span = typename detail::Span<Metric, Dim>::type;
        }
    }
}

#endif
