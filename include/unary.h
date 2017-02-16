#ifndef SPACE_UNARY_H
#define SPACE_UNARY_H

#include "basis.h"
#include "blade.h"
#include "brigand/algorithms/count.hpp"
#include "brigand/sequences/size.hpp"
#include "brigand/types/integer.hpp"
#include <type_traits>

namespace space {
    namespace unary {
        namespace detail {
            template <class Op,
                      class Algebra,
                      template <class...> class Multivector,
                      template <class...> class Basis,
                      class... Blades>
            constexpr auto UnaryOp(Op, const Multivector<Algebra, Basis<Blades...>>& m)
            {
                using Basis_ = Basis<Blades...>;
                return Multivector<Algebra, Basis_>(
                    (Op::Sign(Blades{}) * m[basis::BladeIndex<Basis_, Blades>::value])...);
            }

            template <class Multivector, class Index>
            struct IndexValue {
                auto operator()(const Multivector& m) { return m[Index::value]; }
            };

            struct NoIndex {
                template <class Multivector>
                auto operator()(const Multivector& m)
                {
                    using T = typename Multivector::ScalarValue;
                    return T{0};
                }
            };

            template <class BasisSource, class Blade, class MultivectorSource>
            constexpr auto BasisIndex(const MultivectorSource& m)
            {
                using Index = brigand::index_of<BasisSource, Blade>;
                using Value = std::conditional_t<std::is_same<Index, brigand::no_such_type_>::value,
                                                 NoIndex,
                                                 IndexValue<MultivectorSource, Index>>;
                return Value{}(m);
            }

            template <class MultivectorTarget,
                      class MultivectorSource,
                      template <class...> class TargetBasis,
                      class... TargetBlades>
            constexpr auto CastOp(const MultivectorSource& m,
                                  const TargetBasis<TargetBlades...>& target_basis)
            {
                using BasisSource = typename MultivectorSource::Basis;
                return MultivectorTarget(BasisIndex<BasisSource, TargetBlades>(m)...);
            }
        }

        namespace op {
            template <class T>
            struct Negate {
                template <class Blade>
                static constexpr auto Sign(Blade)
                {
                    return T(-1);
                }
            };

            template <class T>
            struct Reverse {
                template <class Blade>
                static constexpr auto Sign(Blade)
                {
                    return blade::ReverseHasSignFlip(Blade::value) ? T(-1) : T(1);
                }
            };

            template <class T>
            struct Involute {
                template <class Blade>
                static constexpr auto Sign(Blade)
                {
                    return blade::InvoluteHasSignFlip(Blade::value) ? T(-1) : T(1);
                }
            };

            template <class T>
            struct Conjugate {
                template <class Blade>
                static constexpr auto Sign(Blade)
                {
                    return blade::ConjugateHasSignFlip(Blade::value) ? T(-1) : T(1);
                }
            };
        }

        template <class Algebra, template <class...> class Multivector, class Basis>
        constexpr auto Negate(const Multivector<Algebra, Basis>& m)
        {
            using ScalarValue = typename Algebra::ScalarValue;
            return detail::UnaryOp(op::Negate<ScalarValue>{}, m);
        }

        template <class Algebra, template <class...> class Multivector, class Basis>
        constexpr auto Reverse(const Multivector<Algebra, Basis>& m)
        {
            using ScalarValue = typename Algebra::ScalarValue;
            return detail::UnaryOp(op::Reverse<ScalarValue>{}, m);
        }

        template <class Algebra, template <class...> class Multivector, class Basis>
        constexpr auto Involute(const Multivector<Algebra, Basis>& m)
        {
            using ScalarValue = typename Algebra::ScalarValue;
            return detail::UnaryOp(op::Involute<ScalarValue>{}, m);
        }

        template <class Algebra, template <class...> class Multivector, class Basis>
        constexpr auto Conjugate(const Multivector<Algebra, Basis>& m)
        {
            using ScalarValue = typename Algebra::ScalarValue;
            return detail::UnaryOp(op::Conjugate<ScalarValue>{}, m);
        }

        template <class MultivectorTarget, class MultivectorSource>
        constexpr auto Cast(const MultivectorSource& m)
        {
            using BasisTarget = typename MultivectorTarget::Basis;
            return detail::CastOp<MultivectorTarget, MultivectorSource>(m, BasisTarget{});
        }
    }
}

#endif
