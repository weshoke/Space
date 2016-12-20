#ifndef SPACE_BLADE_H
#define SPACE_BLADE_H

#include "brigand/algorithms/fold.hpp"
#include "brigand/algorithms/index_of.hpp"
#include "brigand/algorithms/transform.hpp"
#include "brigand/functions/bitwise/bitxor.hpp"
#include "brigand/functions/if.hpp"
#include "brigand/sequences/back.hpp"
#include "brigand/sequences/front.hpp"
#include <ratio>
#include <type_traits>

namespace space {
    namespace blade {
        namespace detail {
            template <class T>
            static constexpr T Pow(T v, T N)
            {
                return (N > 0) ? (v * Pow(v, N - 1)) : T{1};
            }

            template <class T>
            static constexpr T Grade(T blade, T grade = 0)
            {
                return blade == T{0} ? grade : Grade(blade >> 1, grade + (blade & 1));
            }

            template <class T>
            static constexpr bool HasInner(T a, T b)
            {
                return Grade(a) <= Grade(b) && (Grade(a ^ b) == (Grade(b) - Grade(a)));
            }

            template <class T>
            static constexpr bool HasOuter(T a, T b)
            {
                return !(a & b);
            }

            template <class T>
            static constexpr bool HasSignFlip(T a, T b, T count = 0)
            {
                auto a_shift = a >> 1;
                return a_shift == T{0} ? bool(count & 1)
                                       : HasSignFlip<T>(a_shift, b, count + Grade(a_shift & b));
            }

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
            constexpr bool ReverseHasSignFlip(T a)
            {
                using Signed = std::make_signed_t<T>;
                auto a_signed = Signed(a);
                auto grade = Grade(a_signed);
                return Pow(-1, (grade * (grade - 1) / 2)) == -1;
            }

            template <class T>
            constexpr bool InvoluteHasSignFlip(T a)
            {
                using Signed = std::make_signed_t<T>;
                auto a_signed = Signed(a);
                auto grade = Grade(a_signed);
                return Pow(-1, grade) == -1;
            }

            template <class T>
            constexpr bool ConjugateHasSignFlip(T a)
            {
                using Signed = std::make_signed_t<T>;
                auto a_signed = Signed(a);
                auto grade = Grade(a_signed);
                return Pow(-1, (grade * (grade + 1) / 2)) == -1;
            }

            template <class A, class B>
            struct Scale {
                template <class T>
                static constexpr auto Eval()
                {
                    return T(1);
                };
            };

            template <template <class...> class WeightedA,
                      class A,
                      class WeightA,
                      template <class...> class WeightedB,
                      class B,
                      class WeightB>
            struct Scale<WeightedA<A, WeightA>, WeightedB<B, WeightB>> {
                using type = std::ratio_multiply<WeightA, WeightB>;
            };

            template <class Metric, class A, class B>
            struct ProductScale {
                using IntValue = typename std::make_signed<typename A::value_type>::type;

                static constexpr auto Sign()
                {
                    return HasSignFlip(A::value, B::value) ? IntValue(-1) : IntValue(1);
                }

                using Scale_ = typename Scale<A, B>::type;
                using MetricSign_ =
                    std::ratio<MetricSign<IntValue>(A::value, B::value, Metric::N, Metric::M)>;
                using Sign_ = std::ratio<Sign()>;
                using type = std::ratio_multiply<Scale_, std::ratio_multiply<MetricSign_, Sign_>>;
            };

            template <class A_, class B_>
            struct ProductBlade : public brigand::bitxor_<A_, B_> {
                using A = A_;
                using B = B_;

                static constexpr bool HasOuter() { return detail::HasOuter(A::value, B::value); }
                static constexpr bool HasInner() { return detail::HasInner(A::value, B::value); }
                static constexpr bool HasSignFlip()
                {
                    return detail::HasSignFlip(A::value, B::value);
                }

                template <class Basis>
                static constexpr auto IndexA(Basis)
                {
                    return brigand::index_of<Basis, A>::value;
                }

                template <class Basis>
                static constexpr auto IndexB(Basis)
                {
                    return brigand::index_of<Basis, B>::value;
                }

                template <class T>
                static constexpr auto Sign()
                {
                    return HasSignFlip() ? T(-1) : T(1);
                }

                template <class Metric, class T>
                static constexpr auto Scale()
                {
                    using Scale_ = typename ProductScale<Metric, A, B>::type;
                    return T(Scale_::num) / T(Scale_::den);
                }

                template <class MultivectorA, class MultivectorB>
                static constexpr auto Apply(const MultivectorA& a, const MultivectorB& b)
                {
                    using Metric = typename MultivectorA::Algebra::Metric;
                    using ScalarValue = typename MultivectorA::ScalarValue;
                    using BasisA = typename MultivectorA::Basis;
                    using BasisB = typename MultivectorB::Basis;
                    return Scale<Metric, ScalarValue>() * a[IndexA(BasisA{})] * b[IndexB(BasisB{})];
                }
            };

            template <class Metric, class A, class B>
            struct BitProduct;

            template <template <class...> class Metric,
                      class A,
                      class B,
                      class N,
                      class M,
                      class Xform>
            struct BitProduct<Metric<N, M, Xform>, A, B> {
                using type = decltype(Xform::template xform<Metric<N, M, Xform>, A, B>());
            };

            template <class Blade>
            struct Weight {
                using type = std::ratio<1>;
            };

            template <template <class...> class Blade, class A, class W>
            struct Weight<Blade<A, W>> {
                using type = W;
            };
        }

        template <class V>
        struct Blade {
            using type = std::integral_constant<typename V::value_type, V::value>;
        };

        template <class Metric, class Pair>
        using BitProduct =
            typename detail::BitProduct<Metric, brigand::front<Pair>, brigand::back<Pair>>::type;

        template <class Metric, class Product>
        using ProductScale =
            typename detail::ProductScale<Metric, typename Product::A, typename Product::B>::type;

        template <class Blade, class Weight_>
        struct WeightedBlade : public Blade {
            using Weight = Weight_;
        };

        template <class Blade>
        using Weight = typename detail::Weight<Blade>::type;

        template <class A_, class B_, class Source>
        struct DerivedBlade : public A_ {
            using A = typename Blade<A_>::type;
            using B = typename Blade<B_>::type;

            template <class Basis>
            static constexpr auto IndexA(Basis)
            {
                return brigand::index_of<Basis, A>::value;
            }

            template <class Basis>
            static constexpr auto IndexB(Basis)
            {
                return brigand::index_of<Basis, B>::value;
            }

            template <class MultivectorA, class MultivectorB>
            static constexpr auto Apply(const MultivectorA& a, const MultivectorB& b)
            {
                using ScalarValue = typename MultivectorA::ScalarValue;
                using BasisA = typename MultivectorA::Basis;
                using BasisB = typename MultivectorB::Basis;
                using W = Weight<A_>;
                return ScalarValue(W::num) / ScalarValue(W::den) * a[IndexA(BasisA{})] *
                       b[IndexB(BasisB{})];
            }
        };
    }
}

#endif
