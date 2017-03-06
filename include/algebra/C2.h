#ifndef SPACE_ALGEBRA_C2_H
#define SPACE_ALGEBRA_C2_H

#include "algebra.h"
#include "basis.h"
#include "metric.h"

namespace space {
    namespace algebra {
        template <class Value>
        struct C2 {
            using Xform = basis::xform::Conformal<2, 3>;
            using Metric = space::Metric<3, 1, Xform>;
            using Algebra = space::Algebra<Metric, Value>;
            using VectorBasis = brigand::at_c<typename Algebra::BasisSpan, 1>;

            // Multivectors
            using Scalar = typename Algebra::Scalar;
            using Vector = typename Algebra::Vec;
            using Point = Vector;
            using PseudoScalar = typename Algebra::PseudoScalar;

            // Euclidean
            // TODO: all non-ori/info 1-blades
            // TODO: capitalize blades as multivectors
            // TODO: lowercase blades as integer types
            // TODO: implement multivector.Coord<int>()
            using EuclideanVector = Multivector<
                Algebra,
                brigand::list<brigand::at_c<brigand::at_c<typename Algebra::BasisSpan, 1>, 0>,
                              brigand::at_c<brigand::at_c<typename Algebra::BasisSpan, 1>, 1>>>;
            using No = Multivector<Algebra, brigand::list<brigand::uint16_t<Xform::Ori()>>>;
            using Ni = Multivector<Algebra, brigand::list<brigand::uint16_t<Xform::Inf()>>>;

            // Aliases
            using S = Scalar;
            using Vec = Vector;
            using EVec = EuclideanVector;
            using Ori = No;
            using Inf = Ni;
            using Pnt = Point;
            using Pss = PseudoScalar;
        };
    }
}

#endif
