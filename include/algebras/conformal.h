#ifndef SPACE_ALGEBRA_CONFORMAL_H
#define SPACE_ALGEBRA_CONFORMAL_H

#include "blade/blade.h"
#include "multivector.h"

namespace space {
    namespace algebra {
        // ConformalHelper?
        template <class Algebra>
        struct Conformal {
            using Metric = typename Algebra::Metric;
            using Xform_ = typename Metric::Xform;

            using Ori = Multivector<Algebra, brigand::list<brigand::uint16_t<Xform_::Ori()>>>;
            using Inf = Multivector<Algebra, brigand::list<brigand::uint16_t<Xform_::Inf()>>>;
        };
    }
}

#endif
