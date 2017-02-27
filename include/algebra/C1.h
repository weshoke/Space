#ifndef SPACE_ALGEBRA_C1_H
#define SPACE_ALGEBRA_C1_H

#include "algebra.h"
#include "basis.h"
#include "metric.h"
// #include "product.h"

namespace space {
    namespace algebra {
        template <class Value>
        struct C1 {
            using Xform = basis::xform::Conformal<1, 2>;
            using Metric = space::Metric<2, 1, Xform>;
            using Algebra = space::Algebra<Metric, Value>;
            using VectorBasis = brigand::at_c<typename Algebra::BasisSpan, 1>;

            // Multivectors
            using Scalar = typename Algebra::Scalar;
            using Vector = typename Algebra::Vec;
            using Point = Vector;
            using PseudoScalar = typename Algebra::PseudoScalar;

            // Euclidean
            // TODO: all non-ori/info 1-blades
            using e1 = Multivector<
                Algebra,
                brigand::list<brigand::at_c<brigand::at_c<typename Algebra::BasisSpan, 1>, 0>>>;
            using EuclideanVector = e1;
            using no = Multivector<Algebra, brigand::list<brigand::uint16_t<Xform::Ori()>>>;
            using ni = Multivector<Algebra, brigand::list<brigand::uint16_t<Xform::Inf()>>>;

            // Aliases
            using S = Scalar;
            using Vec = Vector;
            using EVec = EuclideanVector;
            using Ori = no;
            using Inf = ni;
            using Pnt = Point;
            using Pss = PseudoScalar;
        };
    }
}

#endif
