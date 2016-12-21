#ifndef BLADE_DERIVED_BLADE_H
#define BLADE_DERIVED_BLADE_H

#include "blade/blade.h"
#include "blade/predicates.h"
#include "blade/weight.h"
#include "brigand/algorithms/index_of.hpp"
#include <ratio>

namespace space {
    namespace blade {
        namespace detail {
            template <class T, class R>
            static constexpr T Value()
            {
                return T(R::num) / T(R::den);
            }
        }

        template <class Res, class A, class B>
        struct DerivedBlade : public A {
            using BladeA = typename Blade<A>::type;
            using BladeB = typename Blade<B>::type;

            static constexpr bool HasOuter() { return HasOuter(A::value, B::value); }
            static constexpr bool HasInner() { return HasInner(A::value, B::value); }
            template <class MultivectorA, class MultivectorB>
            static constexpr auto Apply(const MultivectorA& a, const MultivectorB& b)
            {
                using ScalarValue = typename MultivectorA::ScalarValue;
                using BasisA = typename MultivectorA::Basis;
                using BasisB = typename MultivectorB::Basis;
                using IndexA = brigand::index_of<BasisA, BladeA>;
                using IndexB = brigand::index_of<BasisB, BladeB>;
                using S = Weight<Res>;
                return detail::Value<ScalarValue, S>() * a[IndexA::value] * b[IndexB::value];
            }
        };
    }
}

#endif