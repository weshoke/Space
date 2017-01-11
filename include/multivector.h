#ifndef SPACE_MULTIVECTOR_H
#define SPACE_MULTIVECTOR_H

#include "basis.h"
#include "brigand/algorithms/for_each.hpp"
#include "brigand/functions/bitwise/bitxor.hpp"
#include "product.h"
#include "sum.h"
#include "unary.h"
#include <array>
#include <cmath>
#include <ostream>

namespace space {
    // Blades are the individual dimensions in a Clifford algebra
    // Multivector is a linear combination of basis blades
    // Grade is the dimension of a given blade
    template <class Algebra_, class Basis_>
    struct Multivector {
        using Algebra = Algebra_;
        using Basis = Basis_;
        using ScalarValue = typename Algebra::ScalarValue;
        using Size = basis::Size<Basis>;

        Multivector()
        : values{}
        {
        }

        // Explicit??
        template <class... Values>
        Multivector(const Values&... v)
        : values{v...}
        {
        }

        auto operator-() const { return unary::Negate(*this); }
        auto operator~() const { return unary::Reverse(*this); }
        auto operator!() const
        {
            auto tmp = ~(*this);
            auto v = ((*this) * tmp)[0];
            return (v == 0) ? tmp : tmp / v;
        }

        template <class MultivectorB>
        auto operator/(const MultivectorB& b) const
        {
            return *this * !b;
        }

        auto operator/(const typename Algebra::ScalarValue& b) const
        {
            return *this * (ScalarValue(1) / b);
        }

        template <class MultivectorB>
        auto operator*(const MultivectorB& b) const
        {
            return product::Gp(*this, b);
        }

        auto operator*(const typename Algebra::ScalarValue& b) const
        {
            return *this * typename Algebra::S(b);
        }

        template <class MultivectorB>
        auto operator^(const MultivectorB& b) const
        {
            return product::Op(*this, b);
        }

        template <class MultivectorB>
        auto operator<=(const MultivectorB& b) const
        {
            return product::Ip(*this, b);
        }

        template <class MultivectorB>
        auto operator+(const MultivectorB& b) const
        {
            return sum::Add(*this, b);
        }

        template <class MultivectorB>
        auto operator-(const MultivectorB& b) const
        {
            return sum::Add(*this, -b);
        }

        auto Dual() const { return (*this) * Algebra::Pss(-1); }
        auto Undual() const { return (*this) * Algebra::Pss(1); }
        auto Weight() const { return (*this <= *this)[0]; }
        auto Norm() const { return std::sqrt(std::max(ScalarValue(0), Weight())); }
        auto Normalized() const
        {
            auto v = std::sqrt(std::abs(Weight()));
            return v == ScalarValue(0) ? Multivector() : (*this / v);
        }

        template <class MultivectorB>
        auto Spin(const MultivectorB& b) const
        {
            return b * (*this) * ~b;
        }

        auto& operator[](const int32_t idx) { return values[idx]; }
        const auto& operator[](const int32_t idx) const { return values[idx]; }
        friend std::ostream& operator<<(std::ostream& os, const Multivector& m)
        {
            auto i = 0u;
            os << "{";
            brigand::for_each<Basis>([&](auto v) {
                using T = brigand::type_from<decltype(v)>;
                if (i != 0u) {
                    os << ", ";
                }
                if (T::value == 0) {
                    os << "s(" << m.values[i] << ")";
                }
                else {
                    os << "e";
                    // TODO: metaprogram this
                    for (auto j = 0u; j < Algebra::Metric::Dim; ++j) {
                        if (T::value & (1 << j)) {
                            os << (j + 1);
                        }
                    }
                    os << "(" << m.values[i] << ")";
                }
                ++i;
            });
            os << "}";
            return os;
        }

        std::array<ScalarValue, Size::value> values;
    };
}

#endif
