#ifndef SPACE_PRODUCT_H
#define SPACE_PRODUCT_H

#include "blade.h"
#include "brigand/functions/bitwise/bitxor.hpp"

namespace space
{
	// Bit representation of a blade product
//	template<class A, class B>
//	struct Product : public brigand::bitxor_<A, B>
//	{
//		using Blade = Blade<typename A::type>;
//	
//		template<class Scalar>
//		static constexpr auto Sign()
//		{
//			return Blade::HasSignFlip(A::value, B::value) ? Scalar(-1) : Scalar(1);
//		}
//	};
	
//	template<class L>
//	struct BladeProduct {};
//	
//	// TODO: use IdxA and IdxB to extract bit values for basis Sign computation
//	// TODO: remove IdxC
//	template<template<class...> class L, class IdxC, class IdxA, class IdxB, class Product>
//	struct BladeProduct<L<IdxC, IdxA, IdxB, Product>>
//	{
//		template<class A, class B>
//		static constexpr auto Eval(const A& a, const B& b)
//		{
//			using Scalar = typename A::Scalar;
//			return Product::template Sign<Scalar>() *
//				a.values[IdxA::value] *
//				b.values[IdxB::value];
//		}
//	};
//	
//	template<class L>
//	struct BladeProductList {};
//	
//	template<template<class ...> class L, class... Ints>
//	struct BladeProductList<L<Ints...>>
//	{
//		template<class A, class B>
//		static constexpr auto Eval(const A& a, const B& b)
//		{
//			using Scalar = typename A::Scalar;
//			// In C++17 can use parameter pack expansion fold
//			auto res = Scalar{0};
//			(void)std::initializer_list<int>{((res += BladeProduct<Ints>::Eval(a, b)), void(), 0)...};
//			return res;
//		}
//	};
//	
//	
//	template<class L, class C>
//	struct MultivectorProduct {};
//	
//	template<template<class...> class L, class... Ints, class C>
//	struct MultivectorProduct<L<Ints...>, C>
//	{
//		template<class A, class B>
//		static constexpr C Eval(const A& a, const B& b)
//		{
//			return C(BladeProductList<Ints>::Eval(a, b)...);
//		}
//	};
}

#endif
