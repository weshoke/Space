#ifndef SPACE_METRIC_H
#define SPACE_METRIC_H

#include "basis/span.h"
#include "basis/xform.h"
#include "brigand/sequences/list.hpp"
#include "brigand/types/integer.hpp"

namespace space {
    namespace detail {
        template <class N_, class M_, class Xform_>
        struct Metric {
            static constexpr auto N = N_::value;
            static constexpr auto M = M_::value;
            static constexpr auto Dim = N + M;
            using Xform = Xform_;
            // using BasisSpan = basis::span::Span<N_, M_, brigand::uint16_t<Dim>>;
        };
    }

    template <uint16_t N, uint16_t M = 0, class Xform = basis::xform::Identity>
    using Metric = detail::Metric<brigand::uint16_t<N>, brigand::uint16_t<M>, Xform>;
}

#endif
