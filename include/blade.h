#ifndef SPACE_BLADE_H
#define SPACE_BLADE_H

#include "brigand/algorithms/index_of.hpp"
#include "brigand/functions/bitwise/bitxor.hpp"
#include "brigand/sequences/back.hpp"
#include "brigand/sequences/front.hpp"

namespace space
{
	namespace blade
	{
		namespace detail
		{
			template<class T>
			static constexpr T Pow(T v, T N)
			{
				return (N > 0) ? (v * Pow(v, N - 1)) : T{1};
			}
			
			template<class T>
			static constexpr T Grade(T blade, T grade = 0) {
			  return blade == T{0} ? grade : Grade(blade >> 1, grade + (blade & 1));
			}
			
			template<class T>
			static constexpr bool HasInner(T a, T b)
			{
				return Grade(a) <= Grade(b) && (Grade(a ^ b) == (Grade(b) - Grade(a)));
			}
			
			template<class T>
			static constexpr bool HasOuter(T a, T b)
			{
				return !(a & b);
			}
			
			template<class T>
			static constexpr bool HasSignFlip(T a, T b, T count = 0)
			{
				auto a_shift = a >> 1;
				return a_shift == T{0} ? bool(count & 1)
					: HasSignFlip<T>(a_shift, b, count + Grade(a_shift & b));
			}
			
			template<class A, class B>
			struct BitProduct : public brigand::bitxor_<A, B>
			{
				static constexpr bool HasOuter() { return detail::HasOuter(A::value, B::value); }
				static constexpr bool HasInner() { return detail::HasInner(A::value, B::value); }
				static constexpr bool HasSignFlip() { return detail::HasSignFlip(A::value, B::value); }
				
				template<class Basis>
				static constexpr auto IndexA(Basis) { return brigand::index_of<Basis, A>::value; }
				
				template<class Basis>
				static constexpr auto IndexB(Basis) { return brigand::index_of<Basis, B>::value; }
			};
		}
		
		template<class V>
		struct Blade : std::integral_constant<typename V::value_type, V::value>
		{};

		template<class Pair>
		using BitProduct = detail::BitProduct<brigand::front<Pair>, brigand::back<Pair>>;
	}

	
////		/// Whether reversion causes a sign flip
////		constexpr bool reverse(type a){
////		return cpow( -1, (grade(a) * (grade(a)-1) / 2.0) ) == -1;
////		}
////		/// Whether involution causes a sign flip
////		constexpr bool involute(type a){
////		return cpow( -1, grade(a) ) == -1;
////		}
////		/// Whether conjugation causes a sign flip
////		constexpr bool conjugate(type a){
////		return cpow( -1,(grade(a) * (grade(a)+1) / 2.0) ) == -1;   
////		}
}

#endif
