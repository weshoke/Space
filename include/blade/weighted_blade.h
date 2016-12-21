#ifndef BLADE_WEIGHTED_BLADE_H
#define BLADE_WEIGHTED_BLADE_H

#include "blade.h"
#include <type_traits>

namespace space {
    namespace blade {
        template <class A, class Weight_>
        struct WeightedBlade : public A {
            static_assert(std::is_same<A, Blade<A>>::value,
                          "Weight can only be applied to a Blade");
            using Weight = Weight_;
        };
    }
}

#endif