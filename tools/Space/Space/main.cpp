#include <cstdint>
#include <array>
#include <utility>
#include <iostream>
#include <cxxabi.h>
#include <sstream>
#include "brigand/adapted/pair.hpp"
#include "brigand/sequences/at.hpp"
#include "brigand/sequences/filled_list.hpp"
#include "brigand/sequences/make_sequence.hpp"
#include "brigand/sequences/map.hpp"
#include "brigand/sequences/at.hpp"
#include "brigand/sequences/has_key.hpp"
#include "brigand/sequences/keys_as_sequence.hpp"
#include "brigand/sequences/values_as_sequence.hpp"
#include "brigand/sequences/front.hpp"
#include "brigand/algorithms/flatten.hpp"
#include "brigand/algorithms/index_of.hpp"
#include "brigand/algorithms/fold.hpp"
#include "brigand/algorithms/transform.hpp"
#include "brigand/algorithms/for_each.hpp"
#include "brigand/algorithms/sort.hpp"
#include "brigand/algorithms/unique.hpp"
#include "brigand/algorithms/product.hpp"
#include "brigand/algorithms/group.hpp"
#include "brigand/algorithms/partition.hpp"
#include "brigand/algorithms/replace.hpp"
#include "brigand/algorithms/count.hpp"
#include "brigand/functions/misc.hpp"
#include "brigand/functions/arithmetic.hpp"
#include "brigand/functions/comparisons.hpp"
#include "brigand/functions/bitwise.hpp"
#include "brigand/functions/logical.hpp"
#include "brigand/functions/eval_if.hpp"

std::string demangle(const char* name) {
    int status = -4; // some arbitrary value to eliminate the compiler warning
    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };
    return (status == 0) ? res.get() : name ;
}

std::string pretty_demangle(const char* name)
{
	auto ss = std::stringstream();
	auto d_name = demangle(name);
	auto it = d_name.begin();
	auto ite = d_name.end();
	auto lvl = 0;
	const auto indent = [&]()
	{
		for(auto i = 0; i < lvl; ++i)
		{
			ss << "  ";
		}
	};
	while(it != ite)
	{
		switch(*it)
		{
			case '<':
				ss << *it;
				ss << "\n";
				++lvl;
				indent();
				break;
				
			case '>':
				ss << "\n";
				--lvl;
				indent();
				ss << *it;
				break;
			
			case ',':
				ss << "\n";
				indent();
				ss << *it;
				break;
				
			default:
				ss << *it;
				break;
		}
		++it;
	}
	return ss.str();
}

namespace space
{
//	static constexpr auto Dim = uint16_t{2};

	template <class T>
	struct Bits
	{
		using Type = T;
	
		/// Grade of blade (count number of "on" bits)
		static constexpr T Grade(T a, T c = 0){
		  auto right = a >> 1;
		  return a != T{0} ?
		  (a & 1 ? Grade(right, c + 1) : Grade(right, c)) : c;
		}
	
		// Flip Check when multiplying two blades a and b
		static constexpr bool HasSignFlip(T a, T b, T c = 0){
		  auto right = a >> 1;
		  return right != T{0} ?
			(HasSignFlip(right, b, c + Grade(right & b))) :
			((c & 1) ? true : false);
		}
		
		static constexpr bool HasInner(T a, T b)
		{
			return !((Grade(a) > Grade(b)) || (Grade(a ^ b) != (Grade(b) - Grade(a))));
		}
		
		static constexpr bool HasOuter(T a, T b)
		{
			return !(a & b);
		}
	};
	
	template <class A, class B>
	struct HasInner : std::integral_constant < bool, Bits<typename A::value_type>::HasInner(A::value, B::value)> {};
	
	template <class A, class B>
	struct HasOuter : std::integral_constant < bool, Bits<typename A::value_type>::HasOuter(A::value, B::value)> {};
	
	
	template<uint16_t N, uint16_t M = 0>
	struct Metric
	{
		static constexpr auto Dim = N + M;
		using Bits = Bits<uint16_t>;
	};
	
	
	template<class A, class B>
	struct Product : public brigand::bitxor_<A, B>
	{
		template<class T>
		static constexpr auto Sign()
		{
			return Bits<uint16_t>::HasSignFlip(A::value, B::value) ? T(-1) : T(1);
		}
	};
	
	template<class L>
	struct BladeProduct {};
	
	template<template<class...> class L, class IdxC, class IdxA, class IdxB, class Product>
	struct BladeProduct<L<IdxC, IdxA, IdxB, Product>>
	{
		template<class A, class B>
		static constexpr auto Eval(const A& a, const B& b)
		{
			using Scalar = typename A::Scalar;
			return Product::template Sign<Scalar>() *
				a.values[IdxA::value] *
				b.values[IdxB::value];
		}
	};
	
	template<class L>
	struct BladeProductList {};
	
	template<template<class ...> class L, class... Ints>
	struct BladeProductList<L<Ints...>>
	{
		template<class A, class B>
		static constexpr auto Eval(const A& a, const B& b)
		{
			using Scalar = typename A::Scalar;
			// In C++17 can use parameter pack expansion fold
			auto res = Scalar{0};
			(void)std::initializer_list<int>{((res += BladeProduct<Ints>::Eval(a, b)), void(), 0)...};
			return res;
		}
	};
	
	
	template<class L, class C>
	struct MultivectorProduct {};
	
	template<template<class...> class L, class... Ints, class C>
	struct MultivectorProduct<L<Ints...>, C>
	{
		template<class A, class B>
		static constexpr C Eval(const A& a, const B& b)
		{
			return C(BladeProductList<Ints>::Eval(a, b)...);
		}
	};
	
	// Blades are the individual dimensions in a Clifford algebra
	// Multivector is a linear combination of basis blades
	// Grade is the dimension of a given blade
	template<class Algebra, class Basis>
	struct Multivector
	{
		static constexpr auto Size = size_t{0};
	};
	
	template<class Algebra, class product, class A, class B>
	auto ProductOp(const A &a, const B &b)
	{
		using instructions = brigand::transform<
			product,
			brigand::bind<
				Product,
				brigand::bind<
					brigand::front,
					brigand::_1
				>,
				brigand::bind<
					brigand::back,
					brigand::_1
				>
			>
		>;

		using ordered_instructions = brigand::sort<instructions>;
		using basis_values = brigand::transform<ordered_instructions, brigand::int_<brigand::_1>>;
		using basis = brigand::unique<basis_values>;
		using mv = Multivector<Algebra, basis>;
		
		using a_bases = brigand::transform<
			ordered_instructions,
			brigand::bind<
				brigand::front,
				brigand::_1
			>
		>;
		using b_bases = brigand::transform<
			ordered_instructions,
			brigand::bind<
				brigand::back,
				brigand::_1
			>
		>;
		
		using mv_indices = typename mv::template Indices<basis_values>;
		using a_indices = typename A::template Indices<a_bases>;
		using b_indices = typename B::template Indices<b_bases>;
		
		// TODO: don't need mv_indices
		using indices = brigand::transform<
			mv_indices,
			a_indices,
			b_indices,
			ordered_instructions,
			brigand::bind<brigand::list, brigand::_1, brigand::_2, brigand::_3, brigand::_4>
		>;
		
		using grouped_instructions = brigand::group<
			indices,
			brigand::bind<brigand::front, brigand::_1>
			>;

		return MultivectorProduct<grouped_instructions, mv>::Eval(a, b);
	}
	

	
	
	template<class Algebra, template<class...> class Basis, class...Elements>
	struct Multivector<Algebra, Basis<Elements...>>
	{
		using Scalar = typename Algebra::Scalar;
		static constexpr auto Size = sizeof...(Elements);
		
		using BasisA = Basis<Elements...>;
		using MultivectorA = Multivector<Algebra, BasisA>;
		
		template<class T>
		using IndexOf = brigand::index_of<BasisA, T>;
		
		template<class Bases>
		using Indices = brigand::transform<
			Bases,
			brigand::bind<
				IndexOf,
				brigand::_1
			>
		>;

		
		Multivector()
		: values{}
		{}
		
		template <class... Values>
        Multivector(const Values&... v)
        : values{v...}
        {
        }
		
		
		
		
		
		template<class BasisB>
		auto operator * (const Multivector<Algebra, BasisB>& b) const
		{
			using MultivectorB = Multivector<Algebra, BasisB>;
			using prod = brigand::product<BasisA, BasisB>;
			return ProductOp<Algebra, prod>(*this, b);
		}
		
		template<class BasisB>
		auto operator ^ (const Multivector<Algebra, BasisB>& b) const
		{
			using MultivectorB = Multivector<Algebra, BasisB>;
			using prod = brigand::product<BasisA, BasisB>;
			using op_prod = brigand::remove_if<
				prod,
				brigand::bind<
					brigand::not_,
					brigand::bind<
						HasOuter,
						brigand::bind<
							brigand::front,
							brigand::_1
						>,
						brigand::bind<
							brigand::back,
							brigand::_1
						>
					>
				>
			>;
			return ProductOp<Algebra, op_prod>(*this, b);
		}
		
		template<class BasisB>
		auto operator <= (const Multivector<Algebra, BasisB>& b) const
		{
			using MultivectorB = Multivector<Algebra, BasisB>;
			using prod = brigand::product<BasisA, BasisB>;
			using op_prod = brigand::remove_if<
				prod,
				brigand::bind<
					brigand::not_,
					brigand::bind<
						HasInner,
						brigand::bind<
							brigand::front,
							brigand::_1
						>,
						brigand::bind<
							brigand::back,
							brigand::_1
						>
					>
				>
			>;
			return ProductOp<Algebra, op_prod>(*this, b);
		}
		
		std::array<Scalar, Size> values;
	};


	template<class Metric, class T>
	struct Algebra
	{
		using Bits = typename Metric::Bits;
		using BitsType = typename Bits::Type;
		
		using Scalar = T;
	
		template<class Basis>
		using Multivector = Multivector<Algebra, Basis>;
		
		using dimensions = brigand::make_sequence<brigand::uint16_t<0>, Metric::Dim>;
		
		// TODO: tuple<Basis0, Basis1, ..., Basis[Dim - 1]>
		using VecBasis = brigand::transform<dimensions, brigand::shift_left<brigand::uint16_t<1>, brigand::_1>>;
		using Vec = Multivector<VecBasis>;
	};
}

using A = space::Algebra<space::Metric<2>, float>;
using Vec = A::Vec;

int main(int argc, const char * argv[])
{
	
	const auto print_vec = [](const auto &v)
	{
		std::cout << "{";
		for(auto i = 0u; i < v.values.size(); ++i)
		{
			if(i != 0)
			{
				std::cout << ", ";
			}
			std::cout << v.values[i];
		}
		std::cout << "}\n";
	};
	
	auto v1 = Vec(0.5f, 0.5f);
	auto v2 = Vec(0.5f, -0.5f);
	auto res = v1 * v2;
	auto res2 = v1 ^ v2;
	auto res3 = v1 <= v2;
	
	print_vec(v1);
	print_vec(v2);
	print_vec(res);
	print_vec(res2);
	print_vec(res3);
	
	
//	std::cout << pretty_demangle(typeid(Gp).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(Gp2).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(Gp3).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(Gp4).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(Gp5).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(Gpa).name()) << "\n";
	return 0;
}
