#ifndef SPACE_BASIS_H
#define SPACE_BASIS_H

#include "blade.h"
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
			
			template<class B1, class B2>
			struct BitProduct
			{
				using type = brigand::transform<
					brigand::product<B1, B2>,
					brigand::bind<
						blade::BitProduct,
						brigand::_1>>;
				
			};
		}
		
		template<class Basis, class Blade>
		using BladeIndex = brigand::index_of<Basis, Blade>;
		
		template<class Basis>
		using Size = brigand::size<Basis>;
		
		// Generate the vector basis
		template<class Algebra>
		using Vectors = typename detail::Vectors<Algebra>::type;

		// Generate the basis pairs for a product from input basis lists
		template<class B1, class B2>
		using BitProduct = typename detail::BitProduct<B1, B2>::type;
	}
}

#endif
