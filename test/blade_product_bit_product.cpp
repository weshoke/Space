#include "basis/xform/conformal.h"
#include "blade/derived_blade.h"
#include "blade/product/bit_product.h"
#include "blade/weight.h"
#include "metric.h"

template <class Metric, uint16_t B1, uint16_t B2>
using BitProductBlade = brigand::front<
    space::blade::BitProduct<Metric, brigand::list<brigand::uint16_t<B1>, brigand::uint16_t<B2>>>>;

template <class Metric, uint16_t B1, uint16_t B2>
using BitProduct =
    space::blade::BitProduct<Metric, brigand::list<brigand::uint16_t<B1>, brigand::uint16_t<B2>>>;

using ME3 = space::Metric<3, 0>;
static_assert(BitProductBlade<ME3, 0b01, 0b01>::value == 0b00, "BitProduct error");
static_assert(BitProductBlade<ME3, 0b01, 0b10>::value == 0b11, "BitProduct error");
static_assert(BitProductBlade<ME3, 0b10, 0b01>::value == 0b11, "BitProduct error");
static_assert(BitProductBlade<ME3, 0b101, 0b01>::value == 0b100, "BitProduct error");

static_assert(BitProductBlade<ME3, 0b01, 0b01>::Scale<float>() == 1.f, "BitProduct error");
static_assert(BitProductBlade<ME3, 0b01, 0b10>::Scale<float>() == 1.f, "BitProduct error");
static_assert(BitProductBlade<ME3, 0b10, 0b01>::Scale<float>() == -1.f, "BitProduct error");
static_assert(BitProductBlade<ME3, 0b101, 0b01>::Scale<float>() == -1.f, "BitProduct error");
static_assert(BitProductBlade<ME3, 0b101, 0b10>::Scale<float>() == -1.f, "BitProduct error");
static_assert(BitProductBlade<ME3, 0b101, 0b100>::Scale<float>() == 1.f, "BitProduct error");

// clang-format off
using MC2 = space::Metric<3, 1, space::basis::xform::Conformal<2, 3>>;
// e1 * e1
static_assert(brigand::size<BitProduct<MC2, 0b01, 0b01>>::value == 1, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b01, 0b01>>::value == 0b00, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b01, 0b01>>::Scale<float>() == 1.f, "BitProduct error");
// e1 * e2
static_assert(brigand::size<BitProduct<MC2, 0b01, 0b10>>::value == 1, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b01, 0b10>>::value == 0b11, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b01, 0b10>>::Scale<float>() == 1.f, "BitProduct error");
// e1 * ori
static_assert(brigand::size<BitProduct<MC2, 0b001, 0b100>>::value == 1, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b001, 0b100>>::value == 0b101, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b001, 0b100>>::Scale<float>() == 1.f, "BitProduct error");
// e1 * inf
static_assert(brigand::size<BitProduct<MC2, 0b0001, 0b1000>>::value == 1, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b0001, 0b1000>>::value == 0b1001, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b0001, 0b1000>>::Scale<float>() == 1.f, "BitProduct error");
// (e1 ^ ori) * e1
static_assert(brigand::size<BitProduct<MC2, 0b0101, 0b0001>>::value == 1, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b0101, 0b0001>>::value == 0b0100, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b0101, 0b0001>>::Scale<float>() == -1.f, "BitProduct error");
// (e1 ^ ori) * ori
static_assert(brigand::size<BitProduct<MC2, 0b0101, 0b0100>>::value == 0, "BitProduct error");
// (e1 ^ inf) * e1
static_assert(brigand::size<BitProduct<MC2, 0b1001, 0b0001>>::value == 1, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b1001, 0b0001>>::value == 0b1000, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b1001, 0b0001>>::Scale<float>() == -1.f, "BitProduct error");
// (e1 ^ inf) * inf
static_assert(brigand::size<BitProduct<MC2, 0b1001, 0b1000>>::value == 0, "BitProduct error");
// ori * inf
static_assert(brigand::size<BitProduct<MC2, 0b0100, 0b1000>>::value == 2, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b0100, 0b1000>>::value == 0b0000, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b0100, 0b1000>>::Scale<float>() == -1.f, "BitProduct error");
static_assert(brigand::back<BitProduct<MC2, 0b0100, 0b1000>>::value == 0b1100, "BitProduct error");
static_assert(brigand::back<BitProduct<MC2, 0b0100, 0b1000>>::Scale<float>() == 1.f, "BitProduct error");
// inf * ori
static_assert(brigand::size<BitProduct<MC2, 0b1000, 0b0100>>::value == 2, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b1000, 0b0100>>::value == 0b0000, "BitProduct error");
static_assert(brigand::front<BitProduct<MC2, 0b1000, 0b0100>>::Scale<float>() == -1.f, "BitProduct error");
static_assert(brigand::back<BitProduct<MC2, 0b1000, 0b0100>>::value == 0b1100, "BitProduct error");
static_assert(brigand::back<BitProduct<MC2, 0b1000, 0b0100>>::Scale<float>() == -1.f, "BitProduct error");
// clang-format on
