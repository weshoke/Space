#ifndef SPACE_VIZ_GEOM_LINE_SEGMENT_H
#define SPACE_VIZ_GEOM_LINE_SEGMENT_H

#include <array>

namespace space {
    namespace geom {
        template <class Vec>
        class LineSegment {
            using T = typename Vec::ScalarValue;

           public:
            LineSegment(const std::array<Vec, 2> &points)
            : points_(points)
            {
            }

            LineSegment(const Vec &start, const Vec &end)
            : points_{start, end}
            {
            }

            Vec Difference() const { return end() - start(); }
            Vec Dir() const { return Difference().Normalized(); }
            Vec Length() const { return Difference().norm(); }
            Vec MidPoint() const { return (start() + end()) * T(0.5); }
            const std::array<Vec, 2> &points() const { return points_; }
            const Vec &start() const { return points_[0]; }
            const Vec &end() const { return points_[1]; }
           private:
            std::array<Vec, 2> points_;
        };
    }
}

#endif
