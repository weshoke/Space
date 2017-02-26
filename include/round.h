#ifndef SPACE_ROUND_H
#define SPACE_ROUND_H

#include "algebra/conformal.h"

namespace space {
    namespace round {
        //	template<class A, class B>
        //    static constexpr GAPnt<A>
        //    center( const Multivector<A,B>& s) {
        //        return  ( s  / ( GAInf<A>(-1) <= s ) ).template cast< GAPnt<A> >();
        //    }
        // TODO: detect dual v. direct
        template <class Elem>
        auto Size(const Elem& elem)
        {
            // static_assert conformal? or put in general conformal section?
            using T = typename Elem::ScalarValue;
            using Algebra = typename Elem::Algebra;
            using Conformal = algebra::Conformal<Algebra>;
            using Inf = typename Conformal::Inf;
            auto s = Inf(T{1}) <= elem;
            // If Grade(elem) <= (Metric.Size - 1) / 2
            // TODO: how does this work in C4D?
            auto is_dual = []() { return true; };
            return (elem * elem.Involute() / (s * s))[0] * (is_dual() ? T{-1} : T{1});
        }
    }
}

#endif
