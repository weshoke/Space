#ifndef BLADE_SCALE_H
#define BLADE_SCALE_H

#include "blade/predicates.h"
#include <ratio>

namespace space {
    namespace blade {
        namespace detail {
            // Product sign based on metric signature
            template <class T>
            static constexpr T MetricSign(T s, T v, T M)
            {
                auto s_next = (v & 1) ? -s : s;
                return M <= 0 ? s : MetricSign(s_next, v >> 1, M - 1);
            }

            template <class T>
            static constexpr T MetricSign(T a, T b, T N, T M)
            {
                auto v = (a & b) >> N;
                return v <= 0 ? 1 : MetricSign(T(1), T(v), M);
            }

            template <class T>
            static constexpr auto Sign(T a, T b)
            {
                return HasSignFlip(a, b) ? T(-1) : T(1);
            }

            // Scale of unweighted blades
            template <class A, class B>
            struct Scale {
                using type = std::ratio<1>;
            };

            // Scale of weighted blades
            template <template <class...> class WeightedA,
                      class A,
                      class WeightA,
                      template <class...> class WeightedB,
                      class B,
                      class WeightB>
            struct Scale<WeightedA<A, WeightA>, WeightedB<B, WeightB>> {
                using type = std::ratio_multiply<WeightA, WeightB>;
            };

            template <class Metric, class Product>
            struct ProductScale;

            template <class Metric, template <class...> class Product, class A, class B>
            struct ProductScale<Metric, Product<A, B>> {
                using Int = typename std::make_signed<typename A::value_type>::type;

                // Weight product
                using Scale_ = typename Scale<A, B>::type;
                // Metric Sign
                using MetricSign_ =
                    std::ratio<MetricSign<Int>(A::value, B::value, Metric::N, Metric::M)>;
                // Blade sign
                using Sign_ = std::ratio<Sign<Int>(A::value, B::value)>;
                using type = std::ratio_multiply<Scale_, std::ratio_multiply<MetricSign_, Sign_>>;
            };

            template <class Metric,
                      template <class...> class DerivedBlade,
                      class Res,
                      class A,
                      class B>
            struct ProductScale<Metric, DerivedBlade<Res, A, B>> {
                using type = typename ProductScale<Metric, brigand::list<A, B>>::type;
            };
        }

        template <class Metric, class Product>
        using ProductScale = typename detail::ProductScale<Metric, Product>::type;
    }
}

#endif
