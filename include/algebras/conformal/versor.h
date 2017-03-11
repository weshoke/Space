#ifndef SPACE_ALGEBRAS_CONFORMAL_VERSOR_H
#define SPACE_ALGEBRAS_CONFORMAL_VERSOR_H

#include <cmath>

namespace space {
    namespace algebras {
        namespace conformal {
            template <class Algebra>
            struct Versor {
                using T = typename Algebra::Value;
                using Inf = typename Algebra::Inf;

                template <class Elem>
                static auto Translator(const Elem &elem)
                {
                    return T{1} - (elem ^ Inf(T{1}) * T{0.5});
                }

                //   Generate Boost as exponential of a Point Pair
                //   Implemented from "Square Root and Logarithm of Rotors. . ." by Dorst and
                //   Valkenburg, 2011
                //   e^B/2 = cosh(B/2) - sinh(B/2)
                //   @param Point Pair generator
                template <class Elem>
                static auto Boost(const Elem &elem)
                {
                    auto weight = elem.Weight();
                    if (weight < T{0}) {
                        auto norm = std::sqrt(-weight);
                        auto im = -std::sin(norm) / norm;
                        auto re = std::cos(norm);
                        return re - elem * im;
                    }
                    else if (weight > T{0}) {
                        auto norm = std::sqrt(weight);
                        auto im = std::sinh(norm) / norm;
                        auto re = std::cosh(norm);
                        return re - elem * im;
                    }
                    return T{1} - elem;
                }
            };
        }
    }
}

#endif
