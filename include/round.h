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
        auto Size(const Elem &elem)
        {
            // static_assert conformal? or put in general conformal section?
            using T = typename Elem::ScalarValue;
            using Algebra = typename Elem::Algebra;
            using Conformal = algebra::Conformal<Algebra>;
            using Inf = typename Conformal::Inf;
            auto s = Inf(T{1}) <= elem;
            auto is_dual = []() { return true; };
            return (elem * elem.Involute() / (s * s))[0] * (is_dual() ? T{-1} : T{1});
        }

        template <class Elem>
        auto Point(const Elem &elem)
        {
            using T = typename Elem::ScalarValue;
            using Algebra = typename Elem::Algebra;
            using Conformal = algebra::Conformal<Algebra>;
            using Ori = typename Conformal::Ori;
            using Inf = typename Conformal::Inf;
            return Ori(T{1}) + Inf(T{0.5} * elem.Weight()) + elem;
        }

        template <class Elem>
        auto DualSphere(const Elem &elem, typename Elem::ScalarValue radius)
        {
            using T = typename Elem::ScalarValue;
            using Algebra = typename Elem::Algebra;
            using Conformal = algebra::Conformal<Algebra>;
            using Inf = typename Conformal::Inf;
            return Point(elem) + Inf(T{0.5} * radius * radius * (radius < 0. ? T{-1} : T{1}));
        }

        template <class Elem>
        auto Split(const Elem &elem)
        {
            using T = typename Elem::ScalarValue;
            using Algebra = typename Elem::Algebra;
            using Conformal = algebra::Conformal<Algebra>;
            using Inf = typename Conformal::Inf;
            using S = typename Algebra::S;
            using Vec = typename Algebra::Vec;
            auto r = std::sqrt(std::abs((elem <= elem)[0]));
            auto d = Inf(T{-1}) <= elem;

            auto a = ((elem + S(r)) / d).template Cast<Vec>();
            auto b = ((elem - S(r)) / d).template Cast<Vec>();
            return std::array<Vec, 2>{a, b};
        }
    }
}

#endif
