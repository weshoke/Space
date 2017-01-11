#include "product.h"
#include "basis/xform/conformal.h"
#include "metric.h"

template <class Metric, class BasisA, class BasisB>
using ProductLists = typename space::product::detail::
    BitProduct<Metric, BasisA, BasisB, brigand::bind<space::product::op::Gp, brigand::_1>>::type;

using MC2 = space::Metric<3, 1, space::basis::xform::Conformal<2, 3>>;
using e1 = brigand::uint16_t<0b0001>;
using e2 = brigand::uint16_t<0b0010>;
using ori = brigand::uint16_t<0b0100>;
using e1_inf = brigand::uint16_t<0b1001>;
using e2_ori = brigand::uint16_t<0b0110>;
using ori_inf = brigand::uint16_t<0b1100>;

// (e1) * (e2)
static_assert(brigand::size<ProductLists<MC2, brigand::list<e1>, brigand::list<e2>>>::value == 1,
              "Product Error");
static_assert(
    brigand::size<brigand::front<ProductLists<MC2, brigand::list<e1>, brigand::list<e2>>>>::value ==
        1,
    "Product Error");
static_assert(brigand::front<
                  brigand::front<ProductLists<MC2, brigand::list<e1>, brigand::list<e2>>>>::value ==
                  0b11,
              "Product Error");
static_assert(
    brigand::front<brigand::front<ProductLists<MC2, brigand::list<e1>, brigand::list<e2>>>>::Scale<
        float>() == 1.f,
    "Product Error");

// (e1 + e2) * (e2)
static_assert(brigand::size<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2>>>::value ==
                  2,
              "Product Error");
static_assert(
    brigand::size<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2>>, 0>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2>>, 0>,
                  0>::value == 0b00,
    "Product Error");
static_assert(
    brigand::at_c<brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2>>, 0>,
                  0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2>>, 1>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2>>, 1>,
                  0>::value == 0b11,
    "Product Error");
static_assert(
    brigand::at_c<brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2>>, 1>,
                  0>::Scale<float>() == 1.f,
    "Product Error");

// (e1 + e2) * (e2 + ori)
static_assert(
    brigand::size<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>>::value == 4,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>,
                                0>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 0>,
        0>::value == 0b00,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 0>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>,
                                1>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 1>,
        0>::value == 0b11,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 1>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>,
                                2>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 2>,
        0>::value == 0b101,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 2>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>,
                                3>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 3>,
        0>::value == 0b110,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 3>,
        0>::Scale<float>() == 1.f,
    "Product Error");

// (e1 + e1^inf) * (e2^ori + ori^inf)
static_assert(
    brigand::size<
        ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>>::value == 5,
    "Product Error");
static_assert(
    brigand::size<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      0>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      0>,
        0>::value == 0b0011,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      0>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      1>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      1>,
        0>::value == 0b0111,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      1>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      2>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      2>,
        0>::value == 0b1001,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      2>,
        0>::Scale<float>() == -1.f,
    "Product Error");
static_assert(
    brigand::size<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      3>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      3>,
        0>::value == 0b1101,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      3>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      4>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      4>,
        0>::value == 0b1111,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductLists<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
                      4>,
        0>::Scale<float>() == 1.f,
    "Product Error");

template <class Metric, class BasisA, class BasisB>
using ProductListsOp = typename space::product::detail::
    BitProduct<Metric, BasisA, BasisB, brigand::bind<space::product::op::Op, brigand::_1>>::type;

// (e1) ^ (e2)
static_assert(brigand::size<ProductListsOp<MC2, brigand::list<e1>, brigand::list<e2>>>::value == 1,
              "Product Error");
static_assert(
    brigand::size<
        brigand::front<ProductListsOp<MC2, brigand::list<e1>, brigand::list<e2>>>>::value == 1,
    "Product Error");
static_assert(
    brigand::front<
        brigand::front<ProductListsOp<MC2, brigand::list<e1>, brigand::list<e2>>>>::value == 0b11,
    "Product Error");
static_assert(brigand::front<brigand::front<
                      ProductListsOp<MC2, brigand::list<e1>, brigand::list<e2>>>>::Scale<float>() ==
                  1.f,
              "Product Error");

// (e1 + e2) ^ (e2)
static_assert(brigand::size<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2>>>::value ==
                  1,
              "Product Error");
static_assert(
    brigand::size<
        brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2>>, 0>>::value ==
        1,
    "Product Error");
static_assert(
    brigand::at_c<brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2>>, 0>,
                  0>::value == 0b11,
    "Product Error");
static_assert(
    brigand::at_c<brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2>>, 0>,
                  0>::Scale<float>() == 1.f,
    "Product Error");

// (e1 + e2) ^ (e2 + ori)
static_assert(
    brigand::size<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>>::value == 3,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>,
                                0>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 0>,
        0>::value == 0b11,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 0>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>,
                                1>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 1>,
        0>::value == 0b101,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 1>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>,
                                2>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 2>,
        0>::value == 0b110,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::at_c<ProductListsOp<MC2, brigand::list<e1, e2>, brigand::list<e2, ori>>, 2>,
        0>::Scale<float>() == 1.f,
    "Product Error");

// (e1 + e1^inf) * (e2^ori + ori^inf)
static_assert(
    brigand::size<
        ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>>::value == 3,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<
            ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
            0>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::
            at_c<ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>, 0>,
        0>::value == 0b0111,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::
            at_c<ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>, 0>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<
            ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
            1>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::
            at_c<ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>, 1>,
        0>::value == 0b1101,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::
            at_c<ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>, 1>,
        0>::Scale<float>() == 1.f,
    "Product Error");
static_assert(
    brigand::size<brigand::at_c<
            ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>,
            2>>::value == 1,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::
            at_c<ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>, 2>,
        0>::value == 0b1111,
    "Product Error");
static_assert(
    brigand::at_c<
        brigand::
            at_c<ProductListsOp<MC2, brigand::list<e1, e1_inf>, brigand::list<e2_ori, ori_inf>>, 2>,
        0>::Scale<float>() == 1.f,
    "Product Error");
