#ifndef SPACE_BLADE_BLADE_H
#define SPACE_BLADE_BLADE_H

#include <type_traits>

namespace space {
    namespace blade {
        namespace detail {
            template <class V>
            struct Blade {
                using type = std::integral_constant<typename V::value_type, V::value>;
            };
        }

        template <class V>
        using Blade = typename detail::Blade<V>::type;
    }
}

#endif
