#ifndef SPACE_ALGEBRAS_CONFORMAL_H
#define SPACE_ALGEBRAS_CONFORMAL_H

#include "algebra.h"
#include "basis.h"
#include "brigand/algorithms/split_at.hpp"
#include "brigand/sequences/at.hpp"
#include "conformal/flat.h"
#include "conformal/round.h"
#include "metric.h"
#include "multivector.h"

namespace space {
    namespace algebras {
        // ConformalHelper?
        template <class Value_, uint16_t EuclideanDim_>
        struct Conformal {
            // Inputs
            using Value = Value_;
            static constexpr auto EuclideanDim = EuclideanDim_;
            using Self = Conformal<Value, EuclideanDim>;

            // Scaffolding
            using Xform = basis::xform::Conformal<EuclideanDim, EuclideanDim + 1>;
            using Metric = space::Metric<EuclideanDim + 1, 1, Xform>;
            using Algebra = space::Algebra<Metric, Value>;

            // Primitive Multivectors
            using S = typename Algebra::S;
            using Vec = typename Algebra::Vec;
            using Pss = typename Algebra::Pss;

            using VectorBasis = typename Vec::Basis;

            template <uint16_t Index>
            using BladeMultivector =
                Multivector<Algebra, brigand::list<brigand::at_c<VectorBasis, Index>>>;

            // Euclidean
            using EuclideanBasis =
                brigand::at_c<brigand::split_at<VectorBasis, brigand::uint16_t<EuclideanDim>>, 0>;
            using EVec = Multivector<Algebra, EuclideanBasis>;

            // Conformal Blades
            using Ori = BladeMultivector<EuclideanDim>;
            using Inf = BladeMultivector<EuclideanDim + 1>;

            // Subspace Operators
            using Flat = conformal::Flat<Self>;
            using Round = conformal::Round<Self>;
        };
    }
}

#endif
