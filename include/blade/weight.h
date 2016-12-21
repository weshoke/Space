#ifndef BLADE_WEIGHT_H
#define BLADE_WEIGHT_H

#include <ratio>

namespace space {
    namespace blade {
        namespace detail {
            template <class Blade>
            struct Weight {
                using type = std::ratio<1>;
            };

            template <template <class...> class Blade, class A, class W>
            struct Weight<Blade<A, W>> {
                using type = W;
            };
        }

        template <class Blade>
        using Weight = typename detail::Weight<Blade>::type;
    }
}

#endif