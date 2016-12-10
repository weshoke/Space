#ifndef SPACE_UNARY_H
#define SPACE_UNARY_H

#include "blade.h"
#include "basis.h"

extern std::string pretty_demangle(const char* name);

namespace space
{
	namespace unary
	{
		namespace detail
		{
			template<
				class Op,
				class Algebra,
				template<class...> class Multivector,
				template<class...> class Basis,
				class... Blades>
			constexpr auto UnaryOp(Op, const Multivector<Algebra, Basis<Blades...>>& m)
			{
				using Basis_ = Basis<Blades...>;
				return Multivector<Algebra, Basis_>(
					(Op::Sign(Blades{}) * m[basis::BladeIndex<Basis_, Blades>::value])...
				);
			}
		}
	
		namespace op
		{
			template<class T>
			struct Reverse
			{
				template<class Blade>
				static constexpr auto Sign(Blade)
				{
					return blade::detail::ReverseHasSignFlip(Blade::value) ? T(-1) : T(1);
				}
			};
			
			template<class T>
			struct Involute
			{
				template<class Blade>
				static constexpr auto Sign(Blade)
				{
					return blade::detail::InvoluteHasSignFlip(Blade::value) ? T(-1) : T(1);
				}
			};
			
			template<class T>
			struct Conjugate
			{
				template<class Blade>
				static constexpr auto Sign(Blade)
				{
					return blade::detail::ConjugateHasSignFlip(Blade::value) ? T(-1) : T(1);
				}
			};
		}
	
		template<
			class Algebra,
			template<class...> class Multivector,
			class Basis>
		constexpr auto Reverse(const Multivector<Algebra, Basis>& m)
		{
			using Scalar = typename Algebra::Scalar;
			return detail::UnaryOp(op::Reverse<Scalar>{}, m);
		}
		
		template<
			class Algebra,
			template<class...> class Multivector,
			class Basis>
		constexpr auto Conjugate(const Multivector<Algebra, Basis>& m)
		{
			using Scalar = typename Algebra::Scalar;
			return detail::UnaryOp(op::Conjugate<Scalar>{}, m);
		}
	}
}

#endif
