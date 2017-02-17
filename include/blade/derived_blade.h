#ifndef BLADE_DERIVED_BLADE_H
#define BLADE_DERIVED_BLADE_H

#include "blade/blade.h"
#include "blade/predicates.h"
#include "blade/weight.h"
#include "brigand/algorithms/index_of.hpp"
#include <ratio>
#include <sstream>

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
        struct DerivedBlade : public Res {
            using BladeA = typename Blade<A>::type;
            using BladeB = typename Blade<B>::type;

            static constexpr bool HasOuter() { return blade::HasOuter(A::value, B::value); }
            static constexpr bool HasInner() { return blade::HasInner(A::value, B::value); }
            template <class T>
            static constexpr T Scale()
            {
                return detail::Value<T, Weight<Res>>();
            }

            template <class MultivectorA, class MultivectorB>
            static constexpr auto Apply(const MultivectorA& a, const MultivectorB& b)
            {
                using ScalarValue = typename MultivectorA::ScalarValue;
                using BasisA = typename MultivectorA::Basis;
                using BasisB = typename MultivectorB::Basis;
                using IndexA = brigand::index_of<BasisA, BladeA>;
                using IndexB = brigand::index_of<BasisB, BladeB>;
                return Scale<ScalarValue>() * a[IndexA::value] * b[IndexB::value];
            }

            template <class MultivectorA, class MultivectorB>
            static auto Print(const MultivectorA& a, const MultivectorB& b)
            {
                using ScalarValue = typename MultivectorA::ScalarValue;
                using BasisA = typename MultivectorA::Basis;
                using BasisB = typename MultivectorB::Basis;
                using IndexA = brigand::index_of<BasisA, BladeA>;
                using IndexB = brigand::index_of<BasisB, BladeB>;
                auto ss = std::stringstream();
                auto v = Scale<ScalarValue>();
                auto v_str = (v == 1) ? "" : ((v == -1) ? "-" : (std::to_string(v) + " * "));
                ss << v_str << "a[" << IndexA::value << "] * b[" << IndexB::value << "]";
                return ss.str();
            }
        };
    }
}

#endif
