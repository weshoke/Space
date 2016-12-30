#include "blade/predicates.h"

// Grade test
static_assert(space::blade::detail::Grade(0) == 0, "Grade error");
static_assert(space::blade::detail::Grade(0b1) == 1, "Grade error");
static_assert(space::blade::detail::Grade(0b10) == 1, "Grade error");
static_assert(space::blade::detail::Grade(0b11) == 2, "Grade error");
static_assert(space::blade::detail::Grade(0b101) == 2, "Grade error");
static_assert(space::blade::detail::Grade(0b1000) == 1, "Grade error");
static_assert(space::blade::detail::Grade(0b1011) == 3, "Grade error");
static_assert(space::blade::detail::Grade(0b1011) == 3, "Grade error");
static_assert(space::blade::detail::Grade(0b101011) == 4, "Grade error");
static_assert(space::blade::detail::Grade(0b1111000) == 4, "Grade error");

// Inner product test
static_assert(space::blade::HasInner(0b01, 0b01) == true, "HasInner error");
static_assert(space::blade::HasInner(0b01, 0b10) == false, "HasInner error");
static_assert(space::blade::HasInner(0b01, 0b11) == true, "HasInner error");
static_assert(space::blade::HasInner(0b11, 0b10) == false, "HasInner error");

// Outer product test
static_assert(space::blade::HasOuter(0b01, 0b01) == false, "HasOuter error");
static_assert(space::blade::HasOuter(0b01, 0b10) == true, "HasOuter error");
// TODO: is this correct?
static_assert(space::blade::HasOuter(0b01, 0b11) == false, "HasOuter error");
static_assert(space::blade::HasOuter(0b11, 0b10) == false, "HasOuter error");

// Check if blade product has a sign flip
static_assert(space::blade::HasSignFlip(0b01, 0b10) == false, "HasSignFlip error");
static_assert(space::blade::HasSignFlip(0b10, 0b01) == true, "HasSignFlip error");
static_assert(space::blade::HasSignFlip(0b01, 0b11) == false, "HasSignFlip error");
static_assert(space::blade::HasSignFlip(0b10, 0b11) == true, "HasSignFlip error");
static_assert(space::blade::HasSignFlip(0b11, 0b01) == true, "HasSignFlip error");
static_assert(space::blade::HasSignFlip(0b11, 0b10) == false, "HasSignFlip error");

// Reverse sign flip
// Involute sign flip
// Conjugate sign flip
