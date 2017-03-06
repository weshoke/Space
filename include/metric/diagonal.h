#ifndef SPACE_METRIC_DIAGONAL_H
#define SPACE_METRIC_DIAGONAL_H

#include "basis/xform.h"

namespace space {
    namespace metric {
        namespace detail {
            template <class Metric>
            struct Diagonal;

            template <template <class...> class Metric, class N, class M, class Xform>
            struct Diagonal<Metric<N, M, Xform>> {
                using type = Metric<N, M, basis::xform::Identity>;
            };
        }

        template <class Metric>
        using Diagonal = typename detail::Diagonal<Metric>::type;
    }
}

#endif
