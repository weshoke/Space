#ifndef SPACE_PRODUCT_H
#define SPACE_PRODUCT_H

#include "basis.h"
#include "brigand/algorithms/group.hpp"
#include "brigand/algorithms/remove.hpp"
#include "brigand/algorithms/sort.hpp"
#include "brigand/algorithms/transform.hpp"
#include "brigand/algorithms/unique.hpp"
#include "brigand/functions/bitwise/bitxor.hpp"
#include "brigand/functions/lambda/bind.hpp"
#include "brigand/functions/logical/not.hpp"

namespace space
{
	namespace product
	{
		namespace detail
		{
			template<class B1, class B2, class Op>
			struct BitProduct
			{
				using Products = brigand::remove_if<
					basis::BitProduct<B1, B2>,
					brigand::bind<
						brigand::not_,
						Op
					>
				>;
				using OrderedProducts = brigand::sort<Products>;
				using OrderedBlades = brigand::transform<OrderedProducts, blade::Blade<brigand::_1>>;
				using ProductLists = brigand::group<
					OrderedProducts,
					brigand::bind<
						basis::BladeIndex,
						brigand::pin<OrderedBlades>,
						brigand::bind<
							blade::Blade,
							brigand::_1
						>
					>
				>;
				using type = ProductLists;
			};
			
			template<
				class BitProduct,
				class Algebra,
				template<class...> class Multivector,
				class BasisA,
				class BasisB>
			constexpr auto BladeProduct(BitProduct, const Multivector<Algebra, BasisA>& a, const Multivector<Algebra, BasisB>& b)
			{
				using A = Multivector<Algebra, BasisA>;
				using ScalarValue = typename A::ScalarValue;
				const auto s = BitProduct::HasSignFlip() ? ScalarValue(-1) : ScalarValue(1);
				return s * a[BitProduct::IndexA(BasisA{})] * b[BitProduct::IndexB(BasisB{})];
			}
			
			template<
				template<class...> class ProductList, class... Products,
				class A,
				class B>
			constexpr auto BasisProduct(ProductList<Products...>, const A& a, const B &b)
			{
				using ScalarValue = typename A::ScalarValue;
				// In C++17 can use parameter pack expansion fold
				auto res = ScalarValue{0};
				(void)std::initializer_list<int>{((res += BladeProduct(Products{}, a, b)), void(), 0)...};
				return res;
			}
			
			template<
				template<class...> class ProductLists, class... Lists,
				class Algebra,
				template<class...> class Multivector,
				class BasisA,
				class BasisB>
			constexpr auto MultivectorProduct(ProductLists<Lists...>, const Multivector<Algebra, BasisA>& a, const Multivector<Algebra, BasisB>& b)
			{
				using ProductBasis = brigand::transform<
					ProductLists<Lists...>,
					brigand::bind<
						brigand::type_from,
						brigand::bind<
							blade::Blade,
							brigand::bind<
								brigand::front,
								brigand::_1
							>
						>
					>
				>;
				return Multivector<Algebra, ProductBasis>(
					BasisProduct(Lists{}, a, b)...
				);
				
			}
		}

		namespace op
		{
			template<class BitProduct>
			struct Gp : std::true_type
			{};
			
			template <class BitProduct>
			struct Op : std::integral_constant<bool, BitProduct::HasOuter()>
			{};
		
			template <class BitProduct>
			struct Ip : std::integral_constant<bool, BitProduct::HasInner()>
			{};
		}
		
		template<
			class Algebra,
			template<class...> class Multivector,
			class BasisA,
			class BasisB>
		constexpr auto Gp(const Multivector<Algebra, BasisA>& a, const Multivector<Algebra, BasisB>& b)
		{
			using ProductLists = typename detail::BitProduct<BasisA, BasisB, op::Gp<brigand::_1>>::type;
			return detail::MultivectorProduct(ProductLists{}, a, b);
		}
		
		template<
			class Algebra,
			template<class...> class Multivector,
			class BasisA>
		constexpr auto Gp(const Multivector<Algebra, BasisA>& a, const typename Algebra::ScalarValue& b)
		{
			return Gp(a, typename Algebra::S(b));
		}
		
		template<
			class Algebra,
			template<class...> class Multivector,
			class BasisA,
			class BasisB>
		constexpr auto Op(const Multivector<Algebra, BasisA>& a, const Multivector<Algebra, BasisB>& b)
		{
			using ProductLists = typename detail::BitProduct<BasisA, BasisB, op::Op<brigand::_1>>::type;
			return detail::MultivectorProduct(ProductLists{}, a, b);
		}
		
		template<
			class Algebra,
			template<class...> class Multivector,
			class BasisA,
			class BasisB>
		constexpr auto Ip(const Multivector<Algebra, BasisA>& a, const Multivector<Algebra, BasisB>& b)
		{
			using ProductLists = typename detail::BitProduct<BasisA, BasisB, op::Ip<brigand::_1>>::type;
			return detail::MultivectorProduct(ProductLists{}, a, b);
		}
	}
}

#endif
