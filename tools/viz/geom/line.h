#ifndef SPACE_VIZ_GEOM_LINE_H
#define SPACE_VIZ_GEOM_LINE_H

#include <array>

namespace space {
    namespace geom {
        template <class Vec>
        class Line {
            using T = typename Vec::ScalarValue;

           public:
            Line(const Vec &start, const Vec &dir)
            : start_(start)
            , dir_(dir)
            {
            }

            Vec Point(T t) const { return start() + dir() * t; }
            const Vec &start() const { return start_; }
            const Vec &dir() const { return dir_; }
           private:
            Vec start_;
            Vec dir_;
        };
    }
}

#endif
