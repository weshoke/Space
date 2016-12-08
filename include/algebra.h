#ifndef SPACE_ALGEBRA_H
#define SPACE_ALGEBRA_H

#include "multivector.h"
#include "basis.h"

namespace space
{
	template<class Metric_, class Scalar_>
	struct Algebra
	{
		using Metric = Metric_;
		using Scalar = Scalar_;
		static constexpr auto Dim = Metric::Dim;
	
		template<class Basis>
		using Multivector = Multivector<Algebra, Basis>;

		using VectorBasis = basis::Vectors<Algebra>;
	};
}

#endif
