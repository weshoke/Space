#ifndef SPACE_ALGEBRAS_CONFORMAL_FLAT_H
#define SPACE_ALGEBRAS_CONFORMAL_FLAT_H

namespace space {
    namespace algebras {
        namespace conformal {
            template <class Algebra>
            struct Flat {
                using T = typename Algebra::Value;
                using Ori = typename Algebra::Ori;
                using Inf = typename Algebra::Inf;
                using Pnt = typename Algebra::Pnt;

                template <class Elem>
                static auto Direction(const Elem &elem)
                {
                    return -Inf(T{1}) <= elem;
                }

                template <class Elem>
                static auto Point(const Elem &elem)
                {
                    return (Ori(T{1}) <= elem) / elem;
                }

                static auto FlatPoint(const Pnt &p) { return p ^ Inf(T{1}); }
                static auto Line(const Pnt &p1, const Pnt &p2) { return p1 ^ p2 ^ Inf(T{1}); }
                static auto Plane(const Pnt &p1, const Pnt &p2, const Pnt &p3)
                {
                    static_assert(Algebra::EuclideanDim >= 2);
                    return p1 ^ p2 ^ p3 ^ Inf(T{1});
                }
            };
        }
    }
}

#endif
