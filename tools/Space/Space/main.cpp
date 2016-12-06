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
	static constexpr auto Dim = uint16_t{2};

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
	};
	
	
	template<uint16_t N, uint16_t M = 0>
	struct Metric
	{
		static constexpr auto Dim = N + M;
		using Bits = Bits<uint16_t>;
	};
	
	template<uint16_t Idx>
	struct Index : public brigand::uint16_t<Idx> {};
	
	template<uint16_t B>
	struct Base : public brigand::uint16_t<B> {};

	template<class List>
	using count_list = brigand::make_sequence<brigand::uint16_t<0>, brigand::size<List>::value>;
	
	template<class B>
	struct make_index
	{
		using type = Index<B::value>;
	};
	
	template<class B>
	struct make_base
	{
		using type = Base<B::value>;
	};
	
	namespace
	{
		template<class List>
		using index_bases_idx = brigand::transform<count_list<List>, make_index<brigand::_1>>;
		
		template<class List>
		using index_bases_bases = brigand::transform<List, make_base<brigand::_1>>;
	}
	
	template<class List>
	struct IndexBases : public
		brigand::type_<brigand::transform<index_bases_idx<List>, index_bases_bases<List>, brigand::pair_wrapper_<brigand::_1, brigand::_2>>>
	{
	
	};
	
	
	template<class A_, class B_>
	struct GpOp : public brigand::bitxor_<A_, B_>
	{
		using A = A_;
		using B = B_;
		
		template<class T>
		static constexpr auto Sign()
		{
			return Bits<uint16_t>::HasSignFlip(A::value, B::value) ? T(-1) : T(1);
		}
	};
	
	template<class V>
	struct to_uint16_t
	{
		using type = brigand::uint16_t<V::value>;
	};
	
//	template<class Indices1>
//	struct Op;
//	
//	template<template<class...> class Indices1, class... Idx1>
//	struct Op<Indices1<Idx1...>>
//	{
//		template<class A>
//		static constexpr auto op(const A& a)
//		{
//			// TODO: use brigand::for_each
//			float res = 0;
//			using expand_variadic_pack  = float[]; // dirty trick, see below
//			(void)expand_variadic_pack{0.f, ((res += Idx1::value), void(), 0.f)... };
//			// first void: silence variable unused warning
//			// uses braced-init-list initialization rules, which evaluates
//			//  the elements inside a braced-init-list IN ORDER, to repetetively
//			//  execute a certain operation
//			// second void is to prevent malicious "operator," overloads, which
//			//  cannot exist for void types
//			// 0 at the end is to handle empty variadic pack (zero-size array initializer is illegal.
//			return res;
//			
//		}
//	};

	
	template<class L>
	struct CalcX {};
	
	template<template<class...> class L, class IdxC, class IdxA, class IdxB, class Product>
	struct CalcX<L<IdxC, IdxA, IdxB, Product>>
	{
		template<class A, class B>
		static constexpr auto Op(const A& a, const B& b)
		{
			using Scalar = typename A::Scalar;
			return Product::template Sign<Scalar>() *
				a.values[IdxA::value] *
				b.values[IdxB::value];
		}
	};
	
	template<class L>
	struct CalcElem {};
	
	template<template<class ...> class L, class... Ints>
	struct CalcElem<L<Ints...>>
	{
		template<class A, class B>
		static constexpr auto Op(const A& a, const B& b)
		{
			using Scalar = typename A::Scalar;
			// In C++17 can use parameter pack expansion fold
			auto res = Scalar{0};
			(void)std::initializer_list<int>{((res += CalcX<Ints>::Op(a, b)), void(), 0)...};
			return res;
		}
	};
	
	
	template<class L, class C>
	struct Calc {};
	
	template<template<class...> class L, class... Ints, class C>
	struct Calc<L<Ints...>, C>
	{
		template<class A, class B>
		static constexpr C Op(const A& a, const B& b)
		{
			return C(CalcElem<Ints>::Op(a, b)...);
		}
	};
	

	// Blades are the individual dimensions in a Clifford algebra
	// Multivector is a linear combination of basis blades
	// Grade is the dimension of a given blade
	template<class Algebra, class Basis>
	struct Multivector {};
	
	template<class Algebra, template<class...> class Basis, class...Elements>
	struct Multivector<Algebra, Basis<Elements...>>
	{
		using Scalar = typename Algebra::Scalar;
		static constexpr auto Size = sizeof...(Elements);
		
		using B1 = Basis<Elements...>;
		
		template<class T>
		using IndexOf = brigand::index_of<B1, T>;
		
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
		
		
		template<template<class...> class Basis2, class...Elements2>
		auto operator * (const Multivector<Algebra, Basis2<Elements2...>>& b) const
		{
			using B2 = Basis2<Elements2...>;
			using MV2 = Multivector<Algebra, B2>;
			using prod = brigand::product<B1, B2>;
			using instructions = brigand::transform<
				prod,
				brigand::bind<
					space::GpOp,
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
			
			using b1_bases = brigand::transform<
				ordered_instructions,
				brigand::bind<
					brigand::front,
					brigand::_1
				>
			>;
			using b2_bases = brigand::transform<
				ordered_instructions,
				brigand::bind<
					brigand::back,
					brigand::_1
				>
			>;
			
			using mv_indices = typename mv::template Indices<basis_values>;
			using b1_indices = Indices<b1_bases>;
			using b2_indices = typename MV2::template Indices<b2_bases>;
			
			using indices = brigand::transform<
				mv_indices,
				b1_indices,
				b2_indices,
				ordered_instructions,
				brigand::bind<brigand::list, brigand::_1, brigand::_2, brigand::_3, brigand::_4>
			>;
			
			using grouped_instructions = brigand::group<
				indices,
				brigand::bind<brigand::front, brigand::_1>
				>;
			
//			std::cout << pretty_demangle(typeid(indices).name()) << "\n";
//			std::cout << "\n";
//			std::cout << pretty_demangle(typeid(grouped_instructions).name()) << "\n";
//			std::cout << "\n";

			return Calc<grouped_instructions, mv>::Op(*this, b);
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



//using namespace brigand;
//
//using L = list<brigand::uint16_t<0>, brigand::uint16_t<0>, brigand::uint16_t<1>, brigand::uint16_t<2>, brigand::uint16_t<2>>;
//using A_ = group<L, less<_1, brigand::uint16_t<2>>>;

int main(int argc, const char * argv[])
{
//	std::cout << pretty_demangle(typeid(L).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(A_).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(sorted).name()) << "\n";
//	std::cout << pretty_demangle(typeid(B_).name()) << "\n";
//	std::cout << pretty_demangle(typeid(X).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(B_).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(C_).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(D_).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(E_).name()) << "\n";
//	std::cout << "\n";
	
	
//	std::cout << pretty_demangle(typeid(X).name()) << "\n";
//	std::cout << "\n";
//	std::cout << pretty_demangle(typeid(decltype(res)).name()) << "\n";
//	std::cout << "\n";
	
	const auto print_vec = [](const auto &v)
	{
		std::cout << "{" << v.values[0] << ", " << v.values[1] << "}\n";
	};
	
	auto v1 = Vec(0.5f, 0.5f);
	auto v2 = Vec(0.5f, -0.5f);
	auto res = v1 * v2;
	
	print_vec(v1);
	print_vec(v2);
	print_vec(res);
	
	
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
