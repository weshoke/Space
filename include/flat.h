#ifndef SPACE_FLAT_H
#define SPACE_FLAT_H

#include "algebras/conformal.h"

namespace space {
    namespace flat {
        template <class Elem>
        auto Direction(const Elem &elem)
        {
            // static_assert conformal? or put in general conformal section?
            using T = typename Elem::ScalarValue;
            using Algebra = typename Elem::Algebra;
            using Conformal = algebra::Conformal<Algebra>;
            using Inf = typename Conformal::Inf;
            return -Inf(T{1}) <= elem;
        }

        template <class Elem>
        auto Point(const Elem &elem)
        {
            // using T = typename Elem::ScalarValue;
            using Algebra = typename Elem::Algebra;
            using Conformal = algebra::Conformal<Algebra>;
            using Ori = typename Conformal::Ori;
            return (Ori(typename Elem::ScalarValue{1}) <= elem) / elem;
        }
    }
}

#endif
