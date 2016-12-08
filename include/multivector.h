#ifndef SPACE_MULTIVECTOR_H
#define SPACE_MULTIVECTOR_H

#include "basis.h"
#include "brigand/functions/bitwise/bitxor.hpp"
#include <array>

namespace space
{
	// Blades are the individual dimensions in a Clifford algebra
	// Multivector is a linear combination of basis blades
	// Grade is the dimension of a given blade
	template<class Algebra, class Basis>
	struct Multivector
	{
		using Scalar = typename Algebra::type;
		using Size = basis::Size<Basis>;
	
		Multivector()
		: values{}
		{}
		
		template <class... Values>
        Multivector(const Values&... v)
        : values{v...}
        {}
		
		// multivector product (with scalars) (list of lists)
		// basis product (with bits + scalars) (list x list) -> list
		// blade product (with bits + scalars) (value x value) -> value
	
		template<class Multivector2>
		auto operator * (const Multivector2 &rhs)
		{
		
		}
		
		std::array<Scalar, Size::value> values;
	};

//	
//
//	
//	
//	template<class Algebra, template<class...> class Basis, class...Elements>
//	struct Multivector<Algebra, Basis<Elements...>>
//	{
//		using Scalar = typename Algebra::Scalar;
//		static constexpr auto Size = sizeof...(Elements);
//		
//		using BasisA = Basis<Elements...>;
//		using MultivectorA = Multivector<Algebra, BasisA>;
//		
//		Multivector()
//		: values{}
//		{}
//		
//		template <class... Values>
//        Multivector(const Values&... v)
//        : values{v...}
//        {
//        }
//		
//		template<class BasisB>
//		auto operator * (const Multivector<Algebra, BasisB>& b) const
//		{
//			using MultivectorB = Multivector<Algebra, BasisB>;
//			using prod = brigand::product<BasisA, BasisB>;
//			return ProductOp<Algebra, prod>(*this, b);
//		}
//		
//		template<class BasisB>
//		auto operator ^ (const Multivector<Algebra, BasisB>& b) const
//		{
//			using MultivectorB = Multivector<Algebra, BasisB>;
//			using prod = brigand::product<BasisA, BasisB>;
//			using op_prod = brigand::remove_if<
//				prod,
//				brigand::bind<
//					brigand::not_,
//					brigand::bind<
//						HasOuter,
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
//			return ProductOp<Algebra, op_prod>(*this, b);
//		}
//		
//		template<class BasisB>
//		auto operator <= (const Multivector<Algebra, BasisB>& b) const
//		{
//			using MultivectorB = Multivector<Algebra, BasisB>;
//			using prod = brigand::product<BasisA, BasisB>;
//			using ip_prod = brigand::remove_if<
//				prod,
//				brigand::bind<
//					brigand::not_,
//					brigand::bind<
//						HasInner,
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
//			return ProductOp<Algebra, ip_prod>(*this, b);
//		}
//		
//		friend std::ostream& operator << (std::ostream& os, const Multivector& m){
//			auto i = 0u;
//			os << "{";
//			brigand::for_each<BasisA>([&](auto v)
//			{
//				using T = brigand::type_from<decltype(v)>;
//				if(i != 0u)
//				{
//					os << ", ";
//				}
//				if(T::value == 0)
//				{
//					os << "s(" << m.values[i] << ")";
//				}
//				else
//				{
//					os << "e";
//					// TODO: metaprogram this
//					for(auto j = 0u; j < Algebra::Dim; ++j)
//					{
//						if(T::value & (1 << j))
//						{
//							os << (j + 1);
//						}
//					}
//					os << "(" << m.values[i] << ")";
//				}
//				++i;
//			});
//			os << "}";
//			return os;
//		}
//		
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
//		
//		std::array<Scalar, Size> values;
//	};
}

#endif
