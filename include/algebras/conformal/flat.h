#ifndef SPACE_ALGEBRAS_CONFORMAL_FLAT_H
#define SPACE_ALGEBRAS_CONFORMAL_FLAT_H

namespace space {
    namespace algebras {
        namespace conformal {
            template <class Algebra>
            struct Flat {
                using T = typename Algebra::Value;
                using Ori = typename Algebra::Ori;
                using Inf = typename Algebra::Inf;

                template <class Elem>
                static auto Direction(const Elem &elem)
                {
                    return -Inf(T{1}) <= elem;
                }

                template <class Elem>
                static auto Point(const Elem &elem)
                {
                    return (Ori(T{1}) <= elem) / elem;
                }
            };
        }
    }
}

#endif
