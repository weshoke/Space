#ifndef SPACE_SUM_H
#define SPACE_SUM_H

#include "brigand/algorithms/index_of.hpp"
#include "brigand/algorithms/transform.hpp"
#include "brigand/functions/lambda/bind.hpp"
#include "brigand/sequences/append.hpp"

namespace space
{
	namespace sum
	{
		namespace detail
		{
			template<class Basis, class Blade, bool First>
			struct Add : public Blade
			{
				static constexpr auto Index() { return brigand::index_of<Basis, Blade>::value; }
				
				template<class A, class B>
				static constexpr auto Value(const A &a, const B &b)
				{
					return First ? a[Index()] : b[Index()];
				}
			};
			
			template<class Basis, class Blade>
			using AddLhs = Add<Basis, Blade, true>;
			
			template<class Basis, class Blade>
			using AddRhs = Add<Basis, Blade, false>;
			
		
			template<class B1, class B2>
			struct BitSum
			{
				using Lhs = brigand::transform<B1,
					brigand::bind<
						AddLhs,
						brigand::pin<B1>,
						brigand::_1
					>
				>;
				using Rhs = brigand::transform<B2,
					brigand::bind<
						AddRhs,
						brigand::pin<B2>,
						brigand::_1
					>
				>;
			
				using Summands = brigand::join<brigand::list<Lhs, Rhs>>;
				using OrderedSummands = brigand::sort<Summands>;
				using OrderedBlades = brigand::transform<OrderedSummands, blade::Blade<brigand::_1>>;
				using SumBasis = brigand::unique<OrderedBlades>;
				using SummandLists = brigand::group<
					OrderedSummands,
					brigand::bind<
						basis::BladeIndex,
						brigand::pin<OrderedBlades>,
						brigand::bind<
							blade::Blade,
							brigand::_1
						>
					>
				>;
				
				using type = SummandLists;
			};
			
			template<
				class Summand,
				class A,
				class B>
			constexpr auto BladeSum(Summand, const A& a, const B& b)
			{
				return Summand::Value(a, b);
			}
			
			template<
				template<class...> class SumList, class... Summands,
				class A,
				class B>
			constexpr auto BasisSum(SumList<Summands...>, const A& a, const B &b)
			{
				using Scalar = typename A::Scalar;
				// In C++17 can use parameter pack expansion fold
				auto res = Scalar{0};
				(void)std::initializer_list<int>{((res += BladeSum(Summands{}, a, b)), void(), 0)...};
				return res;
			}
			
			template<
				template<class...> class SumLists, class... Lists,
				class Algebra,
				template<class...> class Multivector,
				class BasisA,
				class BasisB>
			constexpr auto MultivectorSum(SumLists<Lists...>, const Multivector<Algebra, BasisA>& a, const Multivector<Algebra, BasisB>& b)
			{
				using SumBasis = brigand::transform<
					SumLists<Lists...>,
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
				
				return Multivector<Algebra, SumBasis>(
					BasisSum(Lists{}, a, b)...
				);
			}
		}
	
		template<
			class Algebra,
			template<class...> class Multivector,
			class BasisA,
			class BasisB>
		constexpr auto Add(const Multivector<Algebra, BasisA>& a, const Multivector<Algebra, BasisB>& b)
		{
			using SumLists = typename detail::BitSum<BasisA, BasisB>::type;
			return detail::MultivectorSum(SumLists{}, a, b);
		}
	}
}

#endif
