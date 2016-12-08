#ifndef SPACE_BASIS_H
#define SPACE_BASIS_H

#include "brigand/algorithms/index_of.hpp"
#include "brigand/algorithms/product.hpp"
#include "brigand/functions/bitwise/shift_left.hpp"
#include "brigand/sequences/make_sequence.hpp"
#include "brigand/sequences/size.hpp"
#include <type_traits>

namespace space
{
	namespace basis
	{
		namespace detail
		{
			template<class Algebra>
			struct Vectors
			{
				using type = brigand::transform<
					brigand::make_sequence<brigand::uint16_t<0>, Algebra::Dim>,
					brigand::shift_left<brigand::uint16_t<1>, brigand::_1>>;
			};
		}
		
		template<class Basis, class Blade>
		using BladeIndex = brigand::index_of<Basis, Blade>;
		
		template<class Basis, class Blades>
		using BladeIndices = brigand::transform<
			Blades,
			brigand::bind<
				BladeIndex,
				brigand::pin<Basis>,
				brigand::_1
			>
		>;
		
		template<class Basis>
		using Size = brigand::size<Basis>;
		
		// Generate the vector basis
		template<class Algebra>
		using Vectors = typename detail::Vectors<Algebra>::type;
		
		// Move to basis/product
		template <class A, class B>
		struct Gp : std::true_type
		{};
		
//		template <class A, class B>
//		struct Op: std::integral_constant<bool, Blade<typename A::value_type>::HasOuter(A::value, B::value)> {};
//		{};
//		
//		template <class A, class B>
//		struct Ip: std::integral_constant<bool, Blade<typename A::value_type>::HasInner(A::value, B::value)> {};
//		{};
//		
//		// Generate the basis pairs for a product from input basis lists
//		template<class B1, class B2, class Product = Gp>
//		struct Product
//		{
//			using type = brigand::remove_if<
//				brigand::product<B1, B2>,
//				brigand::bind<
//					brigand::not_,
//					brigand::bind<
//						Product,
//						brigand::bind<
//							brigand::front,
//							brigand::_1
//						>,
//						brigand::bind<
//							brigand::back,
//							brigand::_1
//						>
//					>
//				>
//			>;
//		};
		
		
//		template<class product, class Algebra, class BasisA, class BasisB>
//		auto ProductOp(const Multivector<Algebra, BasisA> &a, const Multivector<Algebra, BasisB> &b)
//		{
//			using instructions = brigand::transform<
//				product,
//				brigand::bind<
//					Product,
//					brigand::bind<
//						brigand::front,
//						brigand::_1
//					>,
//					brigand::bind<
//						brigand::back,
//						brigand::_1
//					>
//				>
//			>;
//
//			using ordered_instructions = brigand::sort<instructions>;
//			using basis_values = brigand::transform<ordered_instructions, brigand::int_<brigand::_1>>;
//			using basis = brigand::unique<basis_values>;
//			using mv = Multivector<Algebra, basis>;
//			
//			using a_bases = brigand::transform<
//				ordered_instructions,
//				brigand::bind<
//					brigand::front,
//					brigand::_1
//				>
//			>;
//			using b_bases = brigand::transform<
//				ordered_instructions,
//				brigand::bind<
//					brigand::back,
//					brigand::_1
//				>
//			>;
//			
//			using mv_indices = typename mv::template Indices<basis_values>;
//			using a_indices = typename A::template Indices<a_bases>;
//			using b_indices = typename B::template Indices<b_bases>;
//			
//			// TODO: don't need mv_indices
//			using indices = brigand::transform<
//				mv_indices,
//				a_indices,
//				b_indices,
//				ordered_instructions,
//				brigand::bind<brigand::list, brigand::_1, brigand::_2, brigand::_3, brigand::_4>
//			>;
//			
//			using grouped_instructions = brigand::group<
//				indices,
//				brigand::bind<brigand::front, brigand::_1>
//				>;
//
//			return MultivectorProduct<grouped_instructions, mv>::Eval(a, b);
//		}
	}
}

#endif
