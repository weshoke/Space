#ifndef SPACE_GEOM_MATRIX3_H
#define SPACE_GEOM_MATRIX3_H

#include "algebras/E3.h"
#include "primitives.h"
#include <array>

// TODO: replace constants with value templates
namespace space {
    namespace geom {
        template <class T>
        class Matrix3 {
           public:
            using E3 = algebras::E3<T>;
            using Vec3 = typename E3::Vec;

            template <class... Values>
            Matrix3(const Values&... v)
            : values_{v...}
            {
            }

            Matrix3(const std::array<T, 9>& values)
            : values_(values)
            {
            }

            friend Matrix3 operator*(const Matrix3& a, const Matrix3& b)
            {
                auto res = std::array<T, 9>{T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}};
                for (auto c = 0u; c < 3u; ++c) {
                    for (auto r = 0u; r < 3u; ++r) {
                        for (auto k = 0u; k < 3u; ++k) {
                            res[Index(c, r)] += a[Index(k, r)] * b[Index(c, k)];
                        }
                    }
                }
                return Matrix4(res);
            }

            friend Vec3 operator*(const Matrix3& a, const Vec3& b)
            {
                auto res = Vec3(T{0}, T{0}, T{0});
                for (auto r = 0u; r < 3u; ++r) {
                    for (auto k = 0u; k < 3u; ++k) {
                        res[r] += a[Index(k, r)] * b[k];
                    }
                }
                return res;
            }

            friend Vec3 operator*(const Vec3& a, const Matrix3& b)
            {
                auto res = Vec3(T{0}, T{0}, T{0});
                for (auto r = 0u; r < 3u; ++r) {
                    for (auto k = 0u; k < 3u; ++k) {
                        res[r] += a[k] * b[Index(r, k)];
                    }
                }
                return res;
            }

            static Matrix3 Identity()
            {
                return Matrix3(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            }

            T& Value(int32_t col, int32_t row) { return values_[Index(col, row)]; }
            T Value(int32_t col, int32_t row) const { return values_[Index(col, row)]; }
            T& operator[](const int32_t idx) { return values_[idx]; }
            T operator[](const int32_t idx) const { return values_[idx]; }
            const std::array<T, 9>& values() const { return values_; }
            friend std::ostream& operator<<(std::ostream& os, const Matrix3& m)
            {
                os << m.Value(0, 0) << "\t" << m.Value(1, 0) << "\t" << m.Value(2, 0) << "\n";
                os << m.Value(0, 1) << "\t" << m.Value(1, 1) << "\t" << m.Value(2, 1) << "\n";
                os << m.Value(0, 2) << "\t" << m.Value(1, 2) << "\t" << m.Value(2, 2) << "\n";
                return os;
            }

           private:
            static inline int32_t Index(int32_t col, int32_t row) { return col * 3 + row; }
            std::array<T, 9> values_;
        };
    }
}

#endif
