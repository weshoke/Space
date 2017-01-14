#include "algebra.h"
#include "basis/xform/conformal.h"
#include "blade.h"
#include "metric.h"
#include "product.h"
#include "sum.h"
#include <OpenGL/gl.h>
#include <cxxabi.h>
#include <array>
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
    auto d_name = demangle(name);
    auto it = d_name.begin();
    auto ite = d_name.end();
    auto lvl = 0;
    const auto indent = [&]() {
        for (auto i = 0; i < lvl; ++i) {
            ss << "  ";
        }
    };
    while (it != ite) {
        switch (*it) {
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

int main(int argc, const char* argv[])
{
    using xform = space::basis::xform::Conformal<2, 3>;
    using MC2 = space::Metric<3, 1, xform>;
    using C2 = space::Algebra<MC2, float>;
    using e1 = brigand::uint16_t<0b0001>;
    using e2 = brigand::uint16_t<0b0010>;
    using ori = brigand::uint16_t<0b0100>;
    using inf = brigand::uint16_t<0b1000>;
    using e1_inf = brigand::uint16_t<0b1001>;
    using e2_ori = brigand::uint16_t<0b0110>;
    using ori_inf = brigand::uint16_t<0b1100>;

    using PT = space::Multivector<C2, brigand::list<e1, e2, ori, inf>>;
    // using MV2 = space::Multivector<C2, brigand::list<e1, e2>>;
    auto p1 = PT(1.f, 0.f, 1.f, 0.5f);
    auto p2 = PT(0.f, 1.f, 1.f, 0.5f);
    auto p3 = PT(1.f, 1.f, 1.f, 1.f);
    auto c = p1 ^ p2 ^ p3;
    // auto x = mv1 * mv2;
    std::cout << p1 << "\n";
    std::cout << p2 << "\n";
    std::cout << p3 << "\n";
    std::cout << c << "\n";

    // using X = ProductListsOp<MC2, brigand::list<e1>, brigand::list<e2>>;
    // using X = ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2>>;
    // using X = ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>;
    // using X = ProductListsOp<MC2, brigand::list<e1_inf>, brigand::list<ori_inf>>;
    // using X = xform::template Apply<MC2, e1_inf, ori_inf, brigand::bind<
    //     space::product::op::Op,
    //     brigand::_1
    // >>;
    // using X = ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>;
    // e1 ^ (e2 * 0.5 * (em + ep))
    // 0.5 * (e1 ^ (e2 * em + e2 * ep))
    // 0.5 * (e1 e2 em + e1 e2 ep)
    // 0.5 * e1 e2 (2 * ori)
    // using X = ProductListsOp<MC2, brigand::list<e1>, brigand::list<e2_ori>>;
    // using X = ProductLists<MC2, brigand::list<e1_inf>, brigand::list<e2_ori>>;
    // using X = BitProductBlade<MC2, 0b1001, 0b0110>;
    // using Y = brigand::at_c<X, 0>;
    // using a = brigand::at_c<Y, 0>;
    // using b = brigand::at_c<Y, 0>;
    // std::cout << pretty_demangle(typeid(L).name()) << "\n";
    // std::cout << pretty_demangle(typeid(M).name()) << "\n";
    // std::cout << pretty_demangle(typeid(M2).name()) << "\n";
    // std::cout << "X>>>>\n";
    // std::cout << pretty_demangle(typeid(X).name()) << "\n";
    // std::cout << pretty_demangle(typeid(Y).name()) << "\n";
    // std::cout << pretty_demangle(typeid(a).name()) << "\n";
    // std::cout << pretty_demangle(typeid(b).name()) << "\n";
    // std::cout << a::HasOuter() << "\n";
    // std::cout << b::HasOuter() << "\n";
    // std::cout << pretty_demangle(typeid(typename X::type_).name()) << "\n";
    // std::cout << Y::HasOuter() << "\n";
    // std::cout << Y::Scale<float>() << "\n";

    // // using B2 = brigand::uint16_t<0b1010>;
    // using P = typename space::blade::detail::BitProduct<MC2, e1, e1>::type;
    // using T = xform::template Apply<MC2, B1, B2>::ProductLists;
    // using Q_ = xform::template Apply<MC2, B1, B2>::Q_;
    // using Q = xform::template Apply<MC2, B1, B2>::Q;
    // using R = xform::template Apply<MC2, B1, B2>::V;
    // // std::cout << pretty_demangle(typeid(T).name()) << "\n";
    // //std::cout << pretty_demangle(typeid(Q).name()) << "\n";
    // std::cout << pretty_demangle(typeid(R).name()) << "\n";
    // std::cout << pretty_demangle(typeid(P).name()) << "\n";

    // using MC2 = space::Metric<3, 1, space::basis::xform::Conformal<2, 3>>;
    // using A1 = BitProductBlade<MC2, 0b101, 0b01>;
    // using A2 = BitProductBlade<MC2, 0b0101, 0b0001>;
    // std::cout << pretty_demangle(typeid(A1).name()) << "\n";
    // std::cout << pretty_demangle(typeid(A2).name()) << "\n";
    // std::cout << brigand::size<A2>::value << "\n";

    // using L1 = brigand::list<e1, e1_inf>;
    // using L2 = brigand::list<e2_ori, ori_inf>;
    // using GG = typename ProductLists<MC2, L1, L2>::type;
    // using HH = typename space::basis::detail::BitProduct<MC2, L1, L2>::Products;
    // using II = brigand::product<L1, L2>;
    // std::cout << pretty_demangle(typeid(GG).name()) << "\n";
    // std::cout << pretty_demangle(typeid(HH).name()) << "\n";
    // std::cout << pretty_demangle(typeid(II).name()) << "\n";

    // using ME2 = space::Metric<2>;
    // using E2 = space::Algebra<ME2, float>;
    // using B1 = brigand::uint16_t<1>;
    // using B2 = brigand::uint16_t<2>;
    // using P = typename space::blade::detail::BitProduct<ME2, B1, B2>::type;
    //
    // using MV1 = space::Multivector<E2, brigand::list<B1>>;
    // using MV2 = space::Multivector<E2, brigand::list<B1, B2>>;
    // auto mv1 = MV1(1.f);
    // auto mv2 = MV2(2.f, 0.5f);
    // auto res = brigand::front<P>::Apply(mv1, mv2);
    //
    // using xx = brigand::front<
    //     space::blade::BitProduct<ME2,
    //                              brigand::list<brigand::uint16_t<0b10>,
    //                              brigand::uint16_t<0b01>>>>;
    // using yy = space::blade::Weight<xx>;
    //
    // std::cout << pretty_demangle(typeid(xx).name()) << "\n";
    // std::cout << pretty_demangle(typeid(yy).name()) << "\n";
    // std::cout << "C: " << xx::Scale<float>() << "\n";
    //    std::cout << res << "\n";

    // using VecBasis = brigand::at_c<typename ME2::BasisSpan, 1>;

    //
    // using BL1 = brigand::front<VecBasis>;
    // using BL2 = brigand::back<VecBasis>;
    // using X = space::blade::detail::BitProduct<ME2, BL1, BL2>;
    //
    // std::cout << pretty_demangle(typeid(typename X::type).name()) << "\n";

    //    using X = space::basis::xform::Conformal<2, 3>;
    //    using C2 = space::Metric<3, 1, X>;
    //    using A = brigand::uint16_t<0b0101>;
    //    using B = brigand::uint16_t<0b1010>;
    //    using DiagonalA = typename X::FromConformal<A>::type;
    //    using DiagonalB = typename X::FromConformal<B>::type;
    //    using ProductLists = space::product::
    //        BitProduct<space::Diagonal<C2>, DiagonalA, DiagonalB,
    //        space::product::op::Gp<brigand::_1>>;
    //    using Q_ = X::ToWeightedBlade<C2, ProductLists>;
    //    using Q = typename X::Simplify<Q_>::type;
    //    using R = typename X::ToConformalList<Q>::type;
    //    using S = brigand::flatten<R>;
    //    using T = typename space::product::detail::GroupBlades<S>::type;
    //    using U = typename X::Simplify<T>::type;
    //    using V = brigand::remove_if<U,
    //                                 brigand::bind<std::ratio_equal,
    //                                               brigand::bind<space::blade::Weight,
    //                                               brigand::_1>,
    //                                               brigand::pin<std::ratio<0>>>>;

    // Group
    // Sum
    //
    //  std::cout << pretty_demangle(typeid(X).name()) << "\n";
    //  std::cout << pretty_demangle(typeid(DiagonalA).name()) << "\n";
    // 	std::cout << pretty_demangle(typeid(DiagonalB).name()) << "\n";
    //    std::cout << "\n\n";
    //    std::cout << pretty_demangle(typeid(ProductLists).name()) << "\n";
    //    std::cout << pretty_demangle(typeid(Q_).name()) << "\n";
    //    std::cout << pretty_demangle(typeid(Q).name()) << "\n";
    //    std::cout << pretty_demangle(typeid(R).name()) << "\n";
    //    std::cout << pretty_demangle(typeid(S).name()) << "\n";
    //    std::cout << pretty_demangle(typeid(T).name()) << "\n";
    //    std::cout << pretty_demangle(typeid(U).name()) << "\n";
    //    std::cout << pretty_demangle(typeid(V).name()) << "\n";
    //    std::cout << A::value << " " << B::value << "\n";
    //	std::cout << pretty_demangle(typeid(decltype(X::xform<C2, A, B>())).name()) << "\n";
    // std::cout << pretty_demangle(typeid(space::basis::span::Span<C2,
    // brigand::uint16_t<C2::Dim>>).name()) << "\n";
    //	std::cout << C2::N << "\n";

    //	using X = space::basis::BitProduct<ME2, VecBasis, VecBasis>;
    //	using Y = space::blade::BitProduct<ME2, brigand::list<BL1, BL2>>;
    //	using Z = typename space::product::detail::BitProduct<ME2, VecBasis, VecBasis,
    // space::product::op::Gp<brigand::_1>>::type;
    //	using Z = typename space::basis::detail::BitProduct2<ME2, VecBasis, VecBasis>::type;
    //	using A =  brigand::remove_if<
    //		Z, brigand::bind<brigand::not_,
    //			space::product::op::Gp<brigand::_1>
    //		>>;
    //	using B = brigand::sort<A>;
    //	using C = brigand::transform<B, space::blade::Blade<brigand::_1>>;
    //	using D = brigand::detail::group<
    //		B,
    //		brigand::bind<
    //			space::basis::BladeIndex,
    //			brigand::pin<C>,
    //			brigand::bind<
    //				brigand::type_from,
    //				brigand::bind<
    //					space::blade::Blade,
    //					brigand::_1
    //				>>>>;
    //
    //	using E = brigand::front<B>;
    //	using F = space::blade::Blade<E>;

    //	template <class Metric, class B1, class B2>
    //            struct BitProduct {
    //                using type = brigand::transform<brigand::product<B1, B2>,
    //                                                brigand::bind<
    //													blade::BitProduct,
    //													//brigand::pin<Metric>,
    //													brigand::_1>>;
    //            };

    // std::cout << pretty_demangle(typeid(E2).name()) << "\n";
    // std::cout << pretty_demangle(typeid(typename E2::MultivectorSpan).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(BL1).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(BL2).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(Y).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(Z).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(A).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(B).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(C).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(typename D::type).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(E).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(F).name()) << "\n";

    //    using Vec = E2::Vec;
    //        auto v1 = Vec(1.f, 0.f);
    //        auto v2 = Vec(1.f, 1.f).Normalized();
    //        auto r = v2 * v1;
    //
    //	using B1 = brigand::uint16_t<0b1001>;
    //	using B2 = brigand::uint16_t<0b0101>;
    //	using C1 = typename space::blade::detail::FromConformal<B1, MC2>::type;
    //	using C2 = typename space::blade::detail::FromConformal<B2, MC2>::type;
    //	// Product of input blades along with source blades
    //	using P = space::product::detail::BitProduct<C1, C2, space::product::op::Gp<brigand::_1>>;
    //	using P_ = typename P::type;
    //	using PP = brigand::transform<
    //		P_,
    //		brigand::bind<
    //			brigand::type_from,
    //			brigand::bind<
    //				space::blade::detail::ProductListSum,
    //				brigand::_1
    //			>
    //		>
    //	>;
    ////	using PR = brigand::transform<
    ////		PP,
    ////		brigand::bind<
    ////			brigand::type_from,
    ////			brigand::bind<
    ////				space::blade::detail::WeightedBladeToConformal,
    ////				brigand::_1
    ////			>
    ////		>
    ////	>;
    //	// Apply ToConformal to each blade and multiply by weight
    //	// Group and sum
    //
    //
    //	std::cout << pretty_demangle(typeid(PP).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(PR).name()) << "\n";

    //	using R = brigand::transform<P,
    //		brigand::bind<
    //			brigand::fold,
    //			brigand::_1,
    //			brigand::pin<std::ratio<0, 0>>,
    //
    //		>
    //	>;
    // reduce groups (sum weights with sign(metric))
    // filter zeros
    // pop result back into conformal space

    //	using PList = brigand::front<typename P::type>;
    //	using WList = brigand::transform<
    //		PList,
    //		brigand::bind<
    //			brigand::type_from,
    //			brigand::bind<
    //				space::blade::detail::Weight,
    //				brigand::_1
    //			>
    //		>
    //	>;
    //	using FF = brigand::remove_if<
    //		typename P::type,
    //		brigand::bind<
    //			std::ratio_equal,
    //			std::ratio<0>,
    //			brigand::bind<
    //				brigand::fold,
    //				brigand::_1,
    //				brigand::pin<std::ratio<0, 1>>,
    //				brigand::defer<
    //					brigand::bind<
    //						std::ratio_add,
    //						brigand::_1,
    //						brigand::bind<
    //							brigand::type_from,
    //							brigand::bind<
    //								space::blade::detail::Weight,
    //								brigand::_2
    //							>
    //						>
    //					>
    //				>
    //			>
    //		>
    //	>;
    //	std::cout << pretty_demangle(typeid(FF).name()) << "\n";
    //
    //	using W = brigand::transform<
    //		typename P::type,
    //		brigand::bind<
    //			brigand::fold,
    //			brigand::_1,
    //			brigand::pin<std::ratio<0, 1>>,
    //			brigand::defer<
    //				brigand::bind<
    //					std::ratio_add,
    //					brigand::_1,
    //					brigand::bind<
    //						brigand::type_from,
    //						brigand::bind<
    //							space::blade::detail::Weight,
    //							brigand::_2
    //						>
    //					>
    //				>
    //			>
    //		>
    //	>;
    //	std::cout << pretty_demangle(typeid(W).name()) << "\n";
    //	if(true) return 0;

    //	std::cout << pretty_demangle(typeid(C1).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(C2).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(typename P::type).name()) << "\n";

    //	std::cout << space::blade::detail::MetricSign(0b1111, 0b1111, 3, 1) << "\n";
    //	std::cout << space::blade::detail::MetricSign(0b1111, 0b1111, 2, 2) << "\n";
    //	std::cout << space::blade::detail::MetricSign(0b1111, 0b1000, 2, 2) << "\n";

    //    std::cout << pretty_demangle(typeid(typename E2::BasisVectors).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(space::basis::DimBasis<E3, 1>).name()) << "\n";
    //	std::cout << pretty_demangle(typeid(typename space::detail::DimBasis<E3, 2>::type).name())
    //<< "\n";
    //	std::cout << pretty_demangle(typeid(typename space::detail::DimBasis<E3, 3>::type).name())
    //<< "\n";
    //	std::cout << pretty_demangle(typeid(decltype(b1)).name()) << "\n";

    //	using X = typename space::sum::detail::BitSum<
    //		E2::VectorBasis,
    //		E2::VectorBasis
    //	>::type;
    //	std::cout << pretty_demangle(typeid(X).name()) << "\n";
    //
    //        std::cout << v1 << "\n";
    //        std::cout << v2 << "\n";
    //        std::cout << r << "\n";
    //        std::cout << v1.Spin(r) << "\n";
    //    	std::cout << (v1 ^ v2) << "\n";
    //    	std::cout << (v1 * v2) << "\n";
    //    	std::cout << (v1 ^ v2) << "\n";
    //        	std::cout << (v1 <= v2) << "\n";
    //    	std::cout << ~v1 << "\n";
    //    	std::cout << (v1 - v2) << "\n";

    //	std::cout << space::blade::detail::InvoluteHasSignFlip(0b01) << "\n";
    //	std::cout << space::blade::detail::InvoluteHasSignFlip(0b10) << "\n";
    //	std::cout << space::blade::detail::InvoluteHasSignFlip(0b11) << "\n";
    //	std::cout << space::blade::detail::InvoluteHasSignFlip(0b110) << "\n";
    //	std::cout << space::blade::detail::InvoluteHasSignFlip(0) << "\n";

    std::cout << GL_LINES << "\n";

    return 0;
}
