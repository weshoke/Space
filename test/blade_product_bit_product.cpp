#include "blade/derived_blade.h"
#include "blade/product/bit_product.h"
#include "blade/weight.h"
#include "metric.h"

using ME3 = space::Metric<3, 0>;

template <uint16_t B1, uint16_t B2>
using BitProductBlade = brigand::front<
    space::blade::BitProduct<ME3, brigand::list<brigand::uint16_t<B1>, brigand::uint16_t<B2>>>>;

static_assert(BitProductBlade<0b01, 0b01>::value == 0b00, "BitProduct error");
static_assert(BitProductBlade<0b01, 0b10>::value == 0b11, "BitProduct error");
static_assert(BitProductBlade<0b10, 0b01>::value == 0b11, "BitProduct error");
static_assert(BitProductBlade<0b101, 0b01>::value == 0b100, "BitProduct error");

static_assert(BitProductBlade<0b01, 0b01>::Scale<float>() == 1.f, "BitProduct error");
static_assert(BitProductBlade<0b01, 0b10>::Scale<float>() == 1.f, "BitProduct error");
static_assert(BitProductBlade<0b10, 0b01>::Scale<float>() == -1.f, "BitProduct error");
static_assert(BitProductBlade<0b101, 0b01>::Scale<float>() == -1.f, "BitProduct error");
static_assert(BitProductBlade<0b101, 0b10>::Scale<float>() == -1.f, "BitProduct error");
static_assert(BitProductBlade<0b101, 0b100>::Scale<float>() == 1.f, "BitProduct error");
