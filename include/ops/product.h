#ifndef SPACE_OPS_PRODUCT_H
#define SPACE_OPS_PRODUCT_H

#include "basis.h"
#include "brigand/algorithms/group.hpp"
#include "brigand/algorithms/remove.hpp"
#include "brigand/algorithms/sort.hpp"
#include "brigand/algorithms/transform.hpp"
#include "brigand/algorithms/unique.hpp"
#include "brigand/functions/bitwise/bitxor.hpp"
#include "brigand/functions/lambda/bind.hpp"
#include "brigand/functions/logical/not.hpp"
#include <initializer_list>
#include <sstream>
#include <vector>

namespace space {
    namespace ops {
        namespace detail {
            template <class List>
            struct GroupBlades {
                using Ordered = brigand::sort<List>;
                // Convert the list elements into a fundamental blade representation
                using Blades =
                    brigand::transform<Ordered, brigand::bind<blade::Blade, brigand::_1>>;
                // Group the list elements by their index in the blade list
                using Grouped =
                    brigand::group<Ordered,
                                   brigand::bind<basis::BladeIndex,
                                                 brigand::pin<Blades>,
                                                 brigand::bind<blade::Blade, brigand::_1>>>;
                using type = Grouped;
            };

            template <class Metric, class B1, class B2, class Op>
            struct BitProduct {
                using Products = basis::BitProduct<Metric, B1, B2, Op>;
                using type = typename GroupBlades<Products>::type;
            };

            template <class BitProduct, class A, class B>
            constexpr auto BladeProduct(BitProduct, const A& a, const B& b)
            {
                return BitProduct::Apply(a, b);
            }

            template <class BitProduct, class A, class B>
            constexpr auto PrintBladeProduct(BitProduct, const A& a, const B& b)
            {
                return BitProduct::Print(a, b);
            }

            template <template <class...> class ProductList, class... Products, class A, class B>
            constexpr auto BasisProduct(ProductList<Products...>, const A& a, const B& b)
            {
                using ScalarValue = typename A::ScalarValue;
                // In C++17 can use parameter pack expansion fold
                auto res = ScalarValue{0};
                (void)std::initializer_list<int>{
                    ((res += BladeProduct(Products{}, a, b)), void(), 0)...};
                return res;
            }

            template <template <class...> class ProductList, class... Products, class A, class B>
            auto PrintBasisProduct(ProductList<Products...>, const A& a, const B& b)
            {
                auto elems = std::vector<std::string>{PrintBladeProduct(Products{}, a, b)...};
                auto ss = std::stringstream();
                for (auto i = 0u; i < elems.size(); ++i) {
                    ss << elems[i];
                    if (i < elems.size() - 1) {
                        ss << " + ";
                    }
                }
                return ss.str();
            }

            template <template <class...> class ProductLists,
                      class... Lists,
                      class Algebra,
                      template <class...> class Multivector,
                      class BasisA,
                      class BasisB>
            constexpr auto MultivectorProduct(ProductLists<Lists...>,
                                              const Multivector<Algebra, BasisA>& a,
                                              const Multivector<Algebra, BasisB>& b)
            {
                using ProductBasis = basis::Basis<ProductLists<Lists...>>;
                return Multivector<Algebra, ProductBasis>(BasisProduct(Lists{}, a, b)...);
            }

            template <template <class...> class ProductLists,
                      class... Lists,
                      class Algebra,
                      template <class...> class Multivector,
                      class BasisA,
                      class BasisB>
            auto PrintMultivectorProduct(ProductLists<Lists...>,
                                         const Multivector<Algebra, BasisA>& a,
                                         const Multivector<Algebra, BasisB>& b)
            {
                auto elems = std::vector<std::string>{PrintBasisProduct(Lists{}, a, b)...};
                auto ss = std::stringstream();
                for (auto i = 0u; i < elems.size(); ++i) {
                    ss << elems[i];
                    if (i < elems.size() - 1) {
                        ss << ",\n";
                    }
                }
                return ss.str();
            }

            template <class BitProduct>
            struct Gp : std::true_type {
            };

            template <class BitProduct>
            struct Op : std::integral_constant<bool, BitProduct::HasOuter()> {
            };

            template <class BitProduct>
            struct Ip : std::integral_constant<bool, BitProduct::HasInner()> {
            };
        }

        template <class Metric, class BasisA, class BasisB, class Op>
        using BitProduct = typename detail::BitProduct<Metric, BasisA, BasisB, Op>::type;

        template <class Metric, class BasisA, class BasisB, class Op>
        using ProductBasis = basis::Basis<BitProduct<Metric, BasisA, BasisB, Op>>;

        template <class Metric, class BasisA, class BasisB>
        using ProductBasisGp = basis::Basis<
            BitProduct<Metric, BasisA, BasisB, brigand::bind<detail::Gp, brigand::_1>>>;

        template <class Algebra, template <class...> class Multivector, class BasisA, class BasisB>
        constexpr auto Gp(const Multivector<Algebra, BasisA>& a,
                          const Multivector<Algebra, BasisB>& b)
        {
            using Metric = typename Algebra::Metric;
            using ProductLists = BitProduct<Metric, BasisA, BasisB, detail::Gp<brigand::_1>>;
            return detail::MultivectorProduct(ProductLists{}, a, b);
        }

        template <class Algebra, template <class...> class Multivector, class BasisA, class BasisB>
        constexpr auto PrintGp(const Multivector<Algebra, BasisA>& a,
                               const Multivector<Algebra, BasisB>& b)
        {
            using Metric = typename Algebra::Metric;
            using ProductLists = BitProduct<Metric, BasisA, BasisB, detail::Gp<brigand::_1>>;
            return detail::PrintMultivectorProduct(ProductLists{}, a, b);
        }

        template <class Metric, class BasisA, class BasisB>
        using ProductBasisOp = basis::Basis<
            BitProduct<Metric, BasisA, BasisB, brigand::bind<detail::Op, brigand::_1>>>;

        template <class Algebra, template <class...> class Multivector, class BasisA, class BasisB>
        constexpr auto Op(const Multivector<Algebra, BasisA>& a,
                          const Multivector<Algebra, BasisB>& b)
        {
            using Metric = typename Algebra::Metric;
            using ProductLists = BitProduct<Metric, BasisA, BasisB, detail::Op<brigand::_1>>;
            return detail::MultivectorProduct(ProductLists{}, a, b);
        }

        template <class Algebra, template <class...> class Multivector, class BasisA, class BasisB>
        constexpr auto PrintOp(const Multivector<Algebra, BasisA>& a,
                               const Multivector<Algebra, BasisB>& b)
        {
            using Metric = typename Algebra::Metric;
            using ProductLists = BitProduct<Metric, BasisA, BasisB, detail::Op<brigand::_1>>;
            return detail::PrintMultivectorProduct(ProductLists{}, a, b);
        }

        template <class Algebra, template <class...> class Multivector, class BasisA, class BasisB>
        constexpr auto Ip(const Multivector<Algebra, BasisA>& a,
                          const Multivector<Algebra, BasisB>& b)
        {
            using Metric = typename Algebra::Metric;
            using ProductLists = BitProduct<Metric, BasisA, BasisB, detail::Ip<brigand::_1>>;
            return detail::MultivectorProduct(ProductLists{}, a, b);
        }
    }
}

#endif
