#ifndef BASIS_XFORM_H
#define BASIS_XFORM_H

#include "blade.h"
#include "brigand/algorithms/flatten.hpp"
#include "product.h"

namespace space {
    namespace basis {
        namespace xform {
            struct Identity {
                template <class Metric, class A, class B>
                struct Apply {
                    using type = brigand::list<blade::Product<Metric, A, B>>;
                };
            };

            template <uint16_t EpIdx, uint16_t Emidx>
            struct Conformal {
                using FlagType = decltype(EpIdx);
                using IntType = std::make_signed<FlagType>;

                static constexpr FlagType Ori() { return 1 << EpIdx; }
                static constexpr FlagType Inf() { return 1 << Emidx; }
                static constexpr FlagType EP() { return 1 << EpIdx; }
                static constexpr FlagType EM() { return 1 << Emidx; }
                static constexpr FlagType EPlane() { return EP() | EM(); }
                static constexpr bool CheckConformal(FlagType a)
                {
                    auto plane = a & EPlane();
                    return !(plane == 0 || plane == EPlane());
                }

                static constexpr bool HasFlag(FlagType a, FlagType flag)
                {
                    return (a & flag) == flag;
                }

                template <class A>
                struct OriToDiagonal {
                    // ori = 1/2 * (em + ep)
                    using type = brigand::list<
                        blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value ^ Ori() ^ EM()>,
                            std::ratio<1, 2>>,
                        blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value ^ Ori() ^ EP()>,
                            std::ratio<1, 2>>>;
                };

                template <class A>
                struct InfToDiagonal {
                    // inf = em - ep
                    using type = brigand::list<
                        blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value ^ Inf() ^ EM()>,
                            std::ratio<1>>,
                        blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value ^ Inf() ^ EP()>,
                            std::ratio<-1>>>;
                };

                template <class A>
                struct EpToConformal {
                    // ep = ori - 1 / 2 * inf
                    using type = brigand::list<
                        blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value ^ EP() ^ Ori()>,
                            std::ratio_multiply<std::ratio<1>, blade::Weight<A>>>,
                        blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value ^ EP() ^ Inf()>,
                            std::ratio_multiply<std::ratio<-1, 2>, blade::Weight<A>>>>;
                };

                template <class A>
                struct EmToConformal {
                    // em = ori + 1 / 2 * inf
                    using type = brigand::list<
                        blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value ^ EM() ^ Ori()>,
                            std::ratio_multiply<std::ratio<1>, blade::Weight<A>>>,
                        blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value ^ EM() ^ Inf()>,
                            std::ratio_multiply<std::ratio<1, 2>, blade::Weight<A>>>>;
                };

                template <class A>
                struct FromConformal {
                    using Conformal = brigand::if_c<HasFlag(A::value, Ori()),
                                                    typename OriToDiagonal<A>::type,
                                                    typename InfToDiagonal<A>::type>;
                    using type = typename brigand::if_c<
                        CheckConformal(A::value),
                        typename Conformal::type,
                        brigand::list<blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value>,
                            blade::Weight<A>>>>::type;
                };

                template <class A>
                struct ToConformal {
                    using Conformal = brigand::if_c<HasFlag(A::value, EP()),
                                                    typename EpToConformal<A>::type,
                                                    typename EmToConformal<A>::type>;
                    using type = typename brigand::if_c<
                        CheckConformal(A::value),
                        typename Conformal::type,
                        brigand::list<blade::WeightedBlade<
                            std::integral_constant<typename A::value_type, A::value>,
                            blade::Weight<A>>>>::type;
                };

                template <class List>
                struct ToConformalList {
                    using type = brigand::transform<
                        List,
                        brigand::bind<brigand::type_from, brigand::bind<ToConformal, brigand::_1>>>;
                };

                template <class Metric, class ProductLists>
                using ToWeightedBlade = brigand::transform<
                    ProductLists,
                    brigand::bind<
                        brigand::transform,
                        brigand::_1,
                        brigand::defer<brigand::bind<blade::WeightedBlade,
                                                     brigand::bind<blade::Blade, brigand::_1>,
                                                     brigand::bind<blade::ProductScale,
                                                                   brigand::pin<Metric>,
                                                                   brigand::_1>>>>>;

                template <class ProductList>
                struct ProductListSum {
                    using WeightSum =
                        brigand::fold<ProductList,
                                      std::ratio<0, 1>,
                                      brigand::bind<std::ratio_add,
                                                    brigand::_1,
                                                    brigand::bind<blade::Weight, brigand::_2>>>;
                    using Elem = brigand::front<ProductList>;
                    using Blade = std::integral_constant<typename Elem::value_type, Elem::value>;
                    using type = blade::WeightedBlade<Blade, WeightSum>;
                };

                template <class ProductLists>
                struct Simplify {
                    using type = brigand::transform<
                        ProductLists,
                        brigand::bind<brigand::type_from,
                                      brigand::bind<ProductListSum, brigand::_1>>>;
                };

                template <class Metric, class A, class B>
                static constexpr auto xform()
                {
                    using DiagonalA = typename FromConformal<A>::type;
                    using DiagonalB = typename FromConformal<B>::type;
                    using ProductLists = product::BitProduct<typename Metric::Diagonal,
                                                             DiagonalA,
                                                             DiagonalB,
                                                             product::op::Gp<brigand::_1>>;
                    using Q_ = ToWeightedBlade<Metric, ProductLists>;
                    using Q = typename Simplify<Q_>::type;
                    using R = typename ToConformalList<Q>::type;
                    using S = brigand::flatten<R>;
                    using T = typename product::detail::GroupBlades<S>::type;
                    using U = typename Simplify<T>::type;
                    using V =
                        brigand::remove_if<U,
                                           brigand::bind<std::ratio_equal,
                                                         brigand::bind<blade::Weight, brigand::_1>,
                                                         brigand::pin<std::ratio<0>>>>;
                    return V{};
                }
            };
        }
    }
}

#endif
