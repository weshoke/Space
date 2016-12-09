#ifndef SPACE_MULTIVECTOR_H
#define SPACE_MULTIVECTOR_H

#include "basis.h"
#include "product.h"
#include "brigand/algorithms/for_each.hpp"
#include "brigand/functions/bitwise/bitxor.hpp"
#include <array>

namespace space
{
	// Blades are the individual dimensions in a Clifford algebra
	// Multivector is a linear combination of basis blades
	// Grade is the dimension of a given blade
	template<class Algebra_, class Basis_>
	struct Multivector
	{
		using Algebra = Algebra_;
		using Basis = Basis_;
		using Scalar = typename Algebra::Scalar;
		using Size = basis::Size<Basis>;
	
		Multivector()
		: values{}
		{}
		
		template <class... Values>
        Multivector(const Values&... v)
        : values{v...}
        {}
		
		template<class MultivectorB>
		auto operator * (const MultivectorB &b)
		{
			return product::Gp(*this, b);
		}
		
		template<class MultivectorB>
		auto operator ^ (const MultivectorB &b)
		{
			return product::Op(*this, b);
		}
		
		template<class MultivectorB>
		auto operator <= (const MultivectorB &b)
		{
			return product::Ip(*this, b);
		}
		
		auto& operator[] (const int32_t idx)
		{
			return values[idx];
		}
		
		const auto& operator[] (const int32_t idx) const
		{
			return values[idx];
		}
		
		friend std::ostream& operator << (std::ostream& os, const Multivector& m)
		{
			auto i = 0u;
			os << "{";
			brigand::for_each<Basis>([&](auto v)
			{
				using T = brigand::type_from<decltype(v)>;
				if(i != 0u)
				{
					os << ", ";
				}
				if(T::value == 0)
				{
					os << "s(" << m.values[i] << ")";
				}
				else
				{
					os << "e";
					// TODO: metaprogram this
					for(auto j = 0u; j < Algebra::Dim; ++j)
					{
						if(T::value & (1 << j))
						{
							os << (j + 1);
						}
					}
					os << "(" << m.values[i] << ")";
				}
				++i;
			});
			os << "}";
			return os;
		}
		
		std::array<Scalar, Size::value> values;
	};

//		// conjugate
//		// involute
//		// invert
//		// reverse
//		// div
//		// add
//		// sub
//		// operator[](basis)
//		// dual
//		// undual
}

#endif
