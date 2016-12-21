#ifndef BLADE_PRODUCT_BIT_PRODUCT_H
#define BLADE_PRODUCT_BIT_PRODUCT_H

#include "brigand/sequences/back.hpp"
#include "brigand/sequences/front.hpp"

namespace space {
    namespace blade {
        namespace detail {
            template <class Metric, class A, class B>
            struct BitProduct;

            template <template <class...> class Metric,
                      class A,
                      class B,
                      class N,
                      class M,
                      class Xform>
            struct BitProduct<Metric<N, M, Xform>, A, B> {
                using type = typename Xform::template Apply<Metric<N, M, Xform>, A, B>::type;
            };
        }

        template <class Metric, class Pair>
        using BitProduct =
            typename detail::BitProduct<Metric, brigand::front<Pair>, brigand::back<Pair>>::type;
    }
}

#endif