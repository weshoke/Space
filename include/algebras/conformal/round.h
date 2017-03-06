#ifndef SPACE_ALGEBRAS_CONFORMAL_ROUND_H
#define SPACE_ALGEBRAS_CONFORMAL_ROUND_H

// #include "algebra.h"
// #include "basis.h"
// #include "metric.h"

#include <array>
#include <cmath>

namespace space {
    namespace algebras {
        namespace conformal {
            template <class Algebra>
            struct Round {
                using T = typename Algebra::Value;
                using Ori = typename Algebra::Ori;
                using Inf = typename Algebra::Inf;
                using S = typename Algebra::S;
                using Vec = typename Algebra::Vec;

                template <class Elem>
                static auto Size(const Elem &elem)
                {
                    auto s = Inf(T{1}) <= elem;
                    auto is_dual = []() { return true; };
                    return (elem * elem.Involute() / (s * s))[0] * (is_dual() ? T{-1} : T{1});
                }

                template <class Elem>
                static auto Point(const Elem &elem)
                {
                    return Ori(T{1}) + Inf(T{0.5} * elem.Weight()) + elem;
                }

                template <class Elem>
                static auto DualSphere(const Elem &elem, typename Elem::ScalarValue radius)
                {
                    return Point(elem) +
                           Inf(T{0.5} * radius * radius * (radius < 0. ? T{1} : T{-1}));
                }

                template <class Elem>
                static auto Split(const Elem &elem)
                {
                    auto r = std::sqrt(std::abs(elem.Weight()));
                    auto d = Inf(T{-1}) <= elem;
                    auto a = ((elem + S(r)) / d).template Cast<Vec>();
                    auto b = ((elem - S(r)) / d).template Cast<Vec>();
                    return std::array<Vec, 2>{a, b};
                }
            };
        }
    }
}

#endif
