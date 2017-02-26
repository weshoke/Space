#include "algebra/E3.h"
#include "algebra/algebra.h"
#include "basis/xform/conformal.h"
#include "blade.h"
#include "metric.h"
#include "product.h"
#include "sum.h"
#include <OpenGL/gl.h>
#include <cxxabi.h>
#include <array>
#include <boost/hana/for_each.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/tuple.hpp>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <utility>

std::string demangle(const char* name)
{
    int status = -4;  // some arbitrary value to eliminate the compiler warning
    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void (*)(void*)> res{abi::__cxa_demangle(name, NULL, NULL, &status),
                                               std::free};
    return (status == 0) ? res.get() : name;
}

std::string pretty_demangle(const char* name)
{
    auto ss = std::stringstream();
    auto lvl = 0;
    const auto indent = [&]() { ss << std::string(lvl * 2, ' '); };
    for (auto c : demangle(name)) {
        switch (c) {
            case '<':
                ss << c << "\n";
                ++lvl;
                indent();
                break;

            case '>':
                ss << "\n";
                --lvl;
                indent();
                ss << c;
                break;

            case ',':
                ss << "\n";
                indent();
                ss << c;
                break;

            default:
                ss << c;
                break;
        }
    }
    return ss.str();
}

// static_assert(space::blade::detail::Grade(0) == 0, "Grade error");
// static_assert(space::blade::detail::Grade(0b1) == 1, "Grade error");
// static_assert(space::blade::detail::Grade(0b10) == 1, "Grade error");
// static_assert(space::blade::detail::Grade(0b11) == 2, "Grade error");
// static_assert(space::blade::detail::Grade(0b101) == 2, "Grade error");
// static_assert(space::blade::detail::Grade(0b1000) == 1, "Grade error");
// static_assert(space::blade::detail::Grade(0b1011) == 3, "Grade error");
//
// static_assert(space::blade::detail::HasInner(0b01, 0b01) == true, "HasInner error");
// static_assert(space::blade::detail::HasInner(0b01, 0b10) == false, "HasInner error");
// static_assert(space::blade::detail::HasInner(0b01, 0b11) == true, "HasInner error");
// static_assert(space::blade::detail::HasInner(0b11, 0b10) == false, "HasInner error");
//
// static_assert(space::blade::detail::HasOuter(0b01, 0b01) == false, "HasOuter error");
// static_assert(space::blade::detail::HasOuter(0b01, 0b10) == true, "HasOuter error");
//// TODO: is this correct?
// static_assert(space::blade::detail::HasOuter(0b01, 0b11) == false, "HasOuter error");
// static_assert(space::blade::detail::HasOuter(0b11, 0b10) == false, "HasOuter error");
//
// static_assert(space::blade::detail::HasSignFlip(0b01, 0b10) == false, "HasSignFlip error");
// static_assert(space::blade::detail::HasSignFlip(0b10, 0b01) == true, "HasSignFlip error");
// static_assert(space::blade::detail::HasSignFlip(0b01, 0b11) == false, "HasSignFlip error");
// static_assert(space::blade::detail::HasSignFlip(0b10, 0b11) == true, "HasSignFlip error");
// static_assert(space::blade::detail::HasSignFlip(0b11, 0b01) == true, "HasSignFlip error");
// static_assert(space::blade::detail::HasSignFlip(0b11, 0b10) == false, "HasSignFlip error");
//
// static_assert(
//    space::blade::detail::BitProduct<brigand::uint16_t<0b01>, brigand::uint16_t<0b01>>::value ==
//        0b00,
//    "BitProduct error");
// static_assert(
//    space::blade::detail::BitProduct<brigand::uint16_t<0b01>, brigand::uint16_t<0b10>>::value ==
//        0b11,
//    "BitProduct error");
// static_assert(
//    space::blade::detail::BitProduct<brigand::uint16_t<0b10>, brigand::uint16_t<0b01>>::value ==
//        0b11,
//    "BitProduct error");
// static_assert(
//    space::blade::detail::BitProduct<brigand::uint16_t<0b101>, brigand::uint16_t<0b01>>::value ==
//        0b100,
//    "BitProduct error");
//
// using E1 = space::Algebra<space::Metric<1>, float>;
// using E2 = space::Algebra<space::Metric<2>, float>;
// using E3 = space::Algebra<space::Metric<3>, float>;
// using E4 = space::Algebra<space::Metric<4>, float>;
// static_assert(std::is_same<typename E2::VectorBasis,
//                           brigand::list<brigand::uint16_t<1>, brigand::uint16_t<2>>>::value,
//              "Vector Basis error");
// static_assert(
//    std::is_same<
//        typename E3::VectorBasis,
//        brigand::list<brigand::uint16_t<1>, brigand::uint16_t<2>, brigand::uint16_t<4>>>::value,
//    "Vector Basis error");
//
//
// using MC2 = space::Metric<3, 1>;

template <class Metric, uint16_t B1, uint16_t B2>
using BitProductBlade =
    space::blade::BitProduct<Metric,
                             brigand::list<brigand::uint16_t<B1>, brigand::uint16_t<B2>>,
                             brigand::bind<space::product::op::Gp, brigand::_1>>;

template <class Metric, uint16_t B1, uint16_t B2>
using BitProductBladeOp =
    space::blade::BitProduct<Metric,
                             brigand::list<brigand::uint16_t<B1>, brigand::uint16_t<B2>>,
                             brigand::bind<space::product::op::Op, brigand::_1>>;

template <class Metric, class BasisA, class BasisB>
using ProductLists = typename space::product::detail::
    BitProduct<Metric, BasisA, BasisB, brigand::bind<space::product::op::Gp, brigand::_1>>::type;

template <class Metric, class BasisA, class BasisB>
using ProductListsOp = typename space::product::detail::
    BitProduct<Metric, BasisA, BasisB, brigand::bind<space::product::op::Op, brigand::_1>>::type;

using E3f = space::algebra::E3<float>;

// constexpr auto Pow2(std::size_t i)
//{
////	return 1 << i;
//	return boost::hana::integral(1 << i);
//}

template <class T>
struct PairT {
    using type = std::pair<T, T>;
};

int main(int argc, const char* argv[])
{
    namespace hana = boost::hana;
    // std::cout << pretty_demangle(typeid(E3f).name()) << "\n";
    //    std::cout << pretty_demangle(typeid(typename E3f::Quat).name()) << "\n";

    //	using P = PairT<int>;
    std::cout << pretty_demangle(typeid(PairT<PairT<PairT<int>>>).name()) << "\n";

    constexpr auto vals = hana::to<hana::tuple_tag>(hana::range_c<std::size_t, 0, 3>);
    //	constexpr auto r = hana::make_range(hana::int_c<0>, hana::int_c<3>);
    //	auto types = hana::make_tuple(std::forward<T>(args)...);
    //	hana::for_each(hana::zip(indices, types), hana::fuse([](auto i, auto&& x) {
    //		// ...
    //	}));
    //
    //	constexpr auto s = hana::transform(vals, [](auto x){ return hana::int_c<1> << x; });

    //	constexpr auto sa = hana::transform(r, [](auto i) {
    ////		return i;
    ////		std::cout << pretty_demangle(typeid(decltype(i)).name()) << "\n";
    //		return hana::int_c<1>;// << 1;
    //	});
    //	constexpr auto s = hana::transform(r, Pow2);
    std::cout << pretty_demangle(typeid(decltype(vals)).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(decltype(s)).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(decltype(hana::integral(1))).name()) << "\n";

    return 0;
}
