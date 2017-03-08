#ifndef SPACE_VIZ_GEOM_LINE_H
#define SPACE_VIZ_GEOM_LINE_H

#include "algebras/conformal.h"
#include <array>

namespace space {
    namespace geom {
        template <class Vec>
        class Line {
            using T = typename Vec::ScalarValue;
            // TODO: convenience template
            using C = algebras::Conformal<T, Vec::Algebra::Metric::EuclideanDim()>;
            using EVec = typename C::EVec;
            using Inf = typename C::Inf;
            using MV = typename C::Line;

           public:
            Line(const Vec &start, const Vec &dir)
            : start_(start)
            , dir_(dir)
            {
            }

            Line(const MV &mv)
                // TODO: better conversion functions
                : start_(C::Flat::Point(mv).template Cast<EVec>().values),
                  dir_(C::Flat::Direction(mv).values)
            {
            }

            Vec Point(T t) const { return start() + dir() * t; }
            // TODO: better conversion functions
            MV Multivector() const
            {
                return C::Round::Point(EVec(start().values)) ^ EVec(dir().values) ^ Inf(T{1});
            }

            const Vec &start() const { return start_; }
            const Vec &dir() const { return dir_; }
           private:
            Vec start_;
            Vec dir_;
        };
    }
}

#endif
