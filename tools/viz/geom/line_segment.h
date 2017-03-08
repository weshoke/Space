#ifndef SPACE_VIZ_GEOM_LINE_SEGMENT_H
#define SPACE_VIZ_GEOM_LINE_SEGMENT_H

#include "algebras/conformal.h"
#include <array>

namespace space {
    namespace geom {
        template <class Vec>
        class LineSegment {
            using T = typename Vec::ScalarValue;
            using C = algebras::Conformal<T, Vec::Algebra::Metric::EuclideanDim()>;
            using EVec = typename C::EVec;
            using MV = typename C::PointPair;

            static std::array<Vec, 2> Split(const MV &mv)
            {
                auto points = C::Round::Split(mv);
                return {points[0].template Cast<EVec>(), points[1].template Cast<EVec>()};
            }

           public:
            LineSegment(const std::array<Vec, 2> &points)
            : points_(points)
            {
            }

            LineSegment(const Vec &start, const Vec &end)
            : points_{start, end}
            {
            }

            LineSegment(const MV &mv)
            : points_(Split(mv))
            {
            }

            Vec Difference() const { return end() - start(); }
            Vec Dir() const { return Difference().Normalized(); }
            Vec Length() const { return Difference().norm(); }
            Vec MidPoint() const { return (start() + end()) * T(0.5); }
            MV Multivector() const
            {
                return C::Round::PointPair(C::Round::Point(EVec(start().values)),
                                           C::Round::Point(EVec(end().values)));
            }

            const std::array<Vec, 2> &points() const { return points_; }
            const Vec &start() const { return points_[0]; }
            const Vec &end() const { return points_[1]; }
           private:
            std::array<Vec, 2> points_;
        };
    }
}

#endif
