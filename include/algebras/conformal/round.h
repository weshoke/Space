#ifndef SPACE_ALGEBRAS_CONFORMAL_ROUND_H
#define SPACE_ALGEBRAS_CONFORMAL_ROUND_H

#include <array>
#include <cmath>

namespace space {
    namespace algebras {
        namespace conformal {
            namespace detail {
                struct NoSuchType {
                    static constexpr uint16_t value = 0;
                };
            }

            template <class Algebra>
            struct Round {
                using T = typename Algebra::Value;
                using Ori = typename Algebra::Ori;
                using Inf = typename Algebra::Inf;
                using S = typename Algebra::S;
                using Vec = typename Algebra::Vec;
                using Pnt = typename Algebra::Pnt;
                using EVec = typename Algebra::EVec;
                using ori = typename Algebra::ori;

                template <class Elem>
                static auto Size(const Elem &elem)
                {
                    auto s = Inf(T{1}) <= elem;
                    constexpr auto is_dual = std::is_same<Elem, Vec>::value;
                    return (elem * elem.Involute() / (s * s))[0] * (is_dual ? T{-1} : T{1});
                }

                template <class Elem>
                static auto Radius(const Elem &elem)
                {
                    auto sz = Size(elem);
                    return sz < T{0} ? -std::sqrt(-sz) : std::sqrt(sz);
                }

                template <class Elem>
                static auto Direction(const Elem &elem)
                {
                    return (Inf(T{1}) <= elem) ^ Inf(T{1});
                }

                template <class Elem>
                static auto Center(const Elem &elem)
                {
                    return (elem / (Inf(T{1}) <= elem)).template Cast<Vec>();
                }

                template <class Elem>
                static auto Point(const Elem &elem)
                {
                    using Basis = typename Elem::Basis;
                    using Index = brigand::index_if<Basis,
                                                    brigand::bind<std::is_same, ori, brigand::_1>,
                                                    detail::NoSuchType>;
                    constexpr auto has_ori = !std::is_same<Index, detail::NoSuchType>::value;
                    auto scale = has_ori ? elem[Index::value] : T{1};
                    auto evec = elem.template Cast<EVec>();
                    return Ori(T{1}) + Inf(T{0.5} * evec.Weight()) + evec / scale;
                }

                template <class Elem>
                static auto CenterPoint(const Elem &elem)
                {
                    return Point(Center(elem));
                }

                template <class Elem>
                static auto DualSphere(const Elem &elem, typename Elem::ScalarValue radius)
                {
                    return Point(elem) +
                           Inf(T{0.5} * radius * radius * (radius < 0. ? T{1} : T{-1}));
                }

                template <class Elem>
                static auto Split(const Elem &elem)
                {
                    auto r = std::sqrt(std::abs(elem.Weight()));
                    auto d = Inf(T{-1}) <= elem;
                    auto a = ((elem + S(r)) / d).template Cast<Vec>();
                    auto b = ((elem - S(r)) / d).template Cast<Vec>();
                    return std::array<Vec, 2>{a, b};
                }

                static auto PointPair(const Pnt &p1, const Pnt &p2) { return p1 ^ p2; }
                static auto Circle(const Pnt &p1, const Pnt &p2, const Pnt &p3)
                {
                    static_assert(Algebra::EuclideanDim >= 2);
                    return p1 ^ p2 ^ p3;
                }

                static auto Sphere(const Pnt &p1, const Pnt &p2, const Pnt &p3, const Pnt &p4)
                {
                    static_assert(Algebra::EuclideanDim >= 3);
                    return p1 ^ p2 ^ p3 ^ p4;
                }
            };
        }
    }
}

#endif
