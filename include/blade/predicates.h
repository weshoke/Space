#ifndef BLADE_PREDICATES_H
#define BLADE_PREDICATES_H

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
        }

        template <class T>
        static constexpr bool HasInner(T a, T b)
        {
            return detail::Grade(a) <= detail::Grade(b) &&
                   (detail::Grade(a ^ b) == (detail::Grade(b) - detail::Grade(a)));
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
                                   : HasSignFlip<T>(a_shift, b, count + detail::Grade(a_shift & b));
        }

        template <class T>
        constexpr bool ReverseHasSignFlip(T a)
        {
            using Signed = std::make_signed_t<T>;
            auto grade = detail::Grade(Signed(a));
            return detail::Pow(-1, (grade * (grade - 1) / 2)) == -1;
        }

        template <class T>
        constexpr bool InvoluteHasSignFlip(T a)
        {
            using Signed = std::make_signed_t<T>;
            auto grade = detail::Grade(Signed(a));
            return detail::Pow(-1, grade) == -1;
        }

        template <class T>
        constexpr bool ConjugateHasSignFlip(T a)
        {
            using Signed = std::make_signed_t<T>;
            auto grade = detail::Grade(Signed(a));
            return detail::Pow(-1, (grade * (grade + 1) / 2)) == -1;
        }
    }
}

#endif