#include <cstdint>
#include <array>
#include <utility>
#include <iostream>
#include <cxxabi.h>
#include <sstream>
#include "blade.h"
#include "algebra.h"
#include "metric.h"

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
//
//namespace space
//{
////	static constexpr auto Dim = uint16_t{2};
//
//	
//	
//	template <class A, class B>
//	struct HasInner : std::integral_constant < bool, Bits<typename A::value_type>::HasInner(A::value, B::value)> {};
//	
//	template <class A, class B>
//	struct HasOuter : std::integral_constant < bool, Bits<typename A::value_type>::HasOuter(A::value, B::value)> {};
//	
//
//
//	template<class Metric, class T>
//	struct Algebra
//	{
//		static constexpr auto Dim = Metric::Dim;
//		using Bits = typename Metric::Bits;
//		using BitsType = typename Bits::Type;
//		
//		using Scalar = T;
//	
//		template<class Basis>
//		using Multivector = Multivector<Algebra, Basis>;
//		
//		using dimensions = brigand::make_sequence<brigand::uint16_t<0>, Metric::Dim>;
//		
//		// TODO: tuple<Basis0, Basis1, ..., Basis[Dim - 1]>
//		using VecBasis = brigand::transform<dimensions, brigand::shift_left<brigand::uint16_t<1>, brigand::_1>>;
//		using Vec = Multivector<VecBasis>;
//	};
//}
//
//using A = space::Algebra<space::Metric<2>, float>;
//using Vec = A::Vec;


static_assert(space::blade::detail::Grade(0) == 0, "Grade error");
static_assert(space::blade::detail::Grade(0b1) == 1, "Grade error");
static_assert(space::blade::detail::Grade(0b10) == 1, "Grade error");
static_assert(space::blade::detail::Grade(0b11) == 2, "Grade error");
static_assert(space::blade::detail::Grade(0b101) == 2, "Grade error");
static_assert(space::blade::detail::Grade(0b1000) == 1, "Grade error");
static_assert(space::blade::detail::Grade(0b1011) == 3, "Grade error");

static_assert(space::blade::detail::HasInner(0b01, 0b01) == true, "HasInner error");
static_assert(space::blade::detail::HasInner(0b01, 0b10) == false, "HasInner error");
static_assert(space::blade::detail::HasInner(0b01, 0b11) == true, "HasInner error");
static_assert(space::blade::detail::HasInner(0b11, 0b10) == false, "HasInner error");

static_assert(space::blade::detail::HasOuter(0b01, 0b01) == false, "HasOuter error");
static_assert(space::blade::detail::HasOuter(0b01, 0b10) == true, "HasOuter error");
// TODO: is this correct?
static_assert(space::blade::detail::HasOuter(0b01, 0b11) == false, "HasOuter error");
static_assert(space::blade::detail::HasOuter(0b11, 0b10) == false, "HasOuter error");

static_assert(space::blade::detail::HasSignFlip(0b01, 0b10) == false, "HasSignFlip error");
static_assert(space::blade::detail::HasSignFlip(0b10, 0b01) == true, "HasSignFlip error");
static_assert(space::blade::detail::HasSignFlip(0b01, 0b11) == false, "HasSignFlip error");
static_assert(space::blade::detail::HasSignFlip(0b10, 0b11) == true, "HasSignFlip error");
static_assert(space::blade::detail::HasSignFlip(0b11, 0b01) == true, "HasSignFlip error");
static_assert(space::blade::detail::HasSignFlip(0b11, 0b10) == false, "HasSignFlip error");


static_assert(
	space::blade::BitProduct<brigand::uint16_t<0b01>, brigand::uint16_t<0b01>>::value == 0b00,
	"BitProduct error");
static_assert(
	space::blade::BitProduct<brigand::uint16_t<0b01>, brigand::uint16_t<0b10>>::value == 0b11,
	"BitProduct error");
static_assert(
	space::blade::BitProduct<brigand::uint16_t<0b10>, brigand::uint16_t<0b01>>::value == 0b11,
	"BitProduct error");
static_assert(
	space::blade::BitProduct<brigand::uint16_t<0b101>, brigand::uint16_t<0b01>>::value == 0b100,
	"BitProduct error");


using E2 = space::Algebra<space::Metric<2>, float>;
using E3 = space::Algebra<space::Metric<3>, float>;
static_assert(std::is_same<
	typename E2::VectorBasis,
	brigand::list<brigand::uint16_t<1>, brigand::uint16_t<2>>
>::value, "Vector Basis error");
static_assert(std::is_same<
	typename E3::VectorBasis,
	brigand::list<brigand::uint16_t<1>, brigand::uint16_t<2>, brigand::uint16_t<4>>
>::value, "Vector Basis error");

int main(int argc, const char * argv[])
{

//	std::cout << 0b10 << "\n";
	// e1 ^ e1 ^ e2
	// e2

//	auto v1 = Vec(0.5f, 0.5f);
//	auto v2 = Vec(0.5f, -0.5f);
//	auto res = v1 * v2;
//	auto res2 = v1 ^ v2;
//	auto res3 = v1 <= v2;
//	
////	print_vec(v1);
////	print_vec(v2);
////	print_vec(res);
////	print_vec(res2);
////	print_vec(res3);
//	std::cout << v1 << "\n";
//	std::cout << v2 << "\n";
//	std::cout << res << "\n";
//	std::cout << res2 << "\n";
//	std::cout << res3 << "\n";
	
	
	std::cout << pretty_demangle(typeid(typename E2::VectorBasis).name()) << "\n";
	std::cout << "\n";
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
