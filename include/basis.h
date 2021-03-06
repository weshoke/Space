#ifndef SPACE_BASIS_H
#define SPACE_BASIS_H

#include "blade.h"
#include "brigand/algorithms/flatten.hpp"
#include "brigand/algorithms/index_of.hpp"
#include "brigand/algorithms/product.hpp"
#include "brigand/algorithms/remove.hpp"
#include "brigand/algorithms/sort.hpp"
#include "brigand/algorithms/transform.hpp"
#include "brigand/algorithms/unique.hpp"
#include "brigand/functions/bitwise/shift_left.hpp"
#include "brigand/functions/comparison/equal_to.hpp"
#include "brigand/sequences/make_sequence.hpp"
#include "brigand/sequences/size.hpp"
#include <type_traits>

namespace space {
    namespace basis {
        namespace detail {
            template <class Metric, class B1, class B2, class Op>
            struct BitProduct {
                using Products = brigand::transform<brigand::product<B1, B2>,
                                                    brigand::bind<blade::BitProduct,
                                                                  brigand::pin<Metric>,
                                                                  brigand::_1,
                                                                  brigand::pin<Op>>>;
                using type = brigand::flatten<Products>;
            };

            template <class T>
            static constexpr T AllBits(T dim, T mask = 0)
            {
                return dim == T{0} ? mask : AllBits(dim - 1, mask << 1 | 0b1);
            }

            template <class ProductLists>
            struct Basis {
                using type = brigand::transform<
                    ProductLists,
                    brigand::bind<blade::Blade, brigand::bind<brigand::front, brigand::_1>>>;
            };
        }

        template <class Basis, class Blade>
        using BladeIndex = brigand::index_of<Basis, Blade>;

        template <class Basis>
        using Size = brigand::size<Basis>;

        template <class Metric, class B1, class B2, class Op>
        using BitProduct = typename detail::BitProduct<Metric, B1, B2, Op>::type;

        template <class ProductLists>
        using Basis = typename detail::Basis<ProductLists>::type;
    }
}

#endif
