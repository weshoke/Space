#ifndef SPACE_BLADE_PRODUCT_H
#define SPACE_BLADE_PRODUCT_H

#include "blade/derived_blade.h"
#include "blade/scale.h"
#include "blade/weighted_blade.h"
#include "brigand/functions/bitwise/bitxor.hpp"

namespace space {
    namespace blade {
        namespace detail {
            template <class Metric, class A, class B>
            struct Product {
                using Scale = blade::ProductScale<Metric, brigand::list<A, B>>;
                using Res = blade::WeightedBlade<blade::Blade<brigand::bitxor_<A, B>>, Scale>;
                using type = blade::DerivedBlade<Res, A, B>;
            };
        }

        template <class Metric, class A, class B>
        using Product = typename detail::Product<Metric, A, B>::type;
    }
}

#endif
