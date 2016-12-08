#ifndef SPACE_METRIC_H
#define SPACE_METRIC_H

#include "brigand/sequences/list.hpp"
#include "brigand/types/integer.hpp"

namespace space
{
	template<uint16_t N, uint16_t M = 0>
	struct Metric
	{
		static constexpr auto Dim = N + M;
		using Signature = brigand::list<brigand::uint16_t<N>, brigand::uint16_t<M>>;
	};
}

#endif
