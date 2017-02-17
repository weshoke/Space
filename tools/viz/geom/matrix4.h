#ifndef SPACE_GEOM_MATRIX4_H
#define SPACE_GEOM_MATRIX4_H

#include <array>

// TODO: replace constants with value templates
namespace space {
    namespace geom {
        template <class T>
        class Matrix4 {
           public:
            template <class... Values>
            Matrix4(const Values&... v)
            : values_{v...}
            {
            }

            Matrix4(const std::array<T, 16>& values)
            : values_(values)
            {
            }

            friend Matrix4 operator*(const Matrix4& a, const Matrix4& b)
            {
                auto res = std::array<T, 16>{
                    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
                for (auto c = 0u; c < 4u; ++c) {
                    for (auto r = 0u; r < 4u; ++r) {
                        for (auto k = 0u; k < 4u; ++k) {
                            res[Index(c, r)] += a[Index(k, r)] * b[Index(c, k)];
                        }
                    }
                }
                return Matrix4(res);
            }

            static Matrix4 Identity()
            {
                return Matrix4(
                    1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
            }

            static Matrix4 Ortho(T left, T right, T bottom, T top, T near, T far)
            {
                auto dx = right - left;
                auto dy = top - bottom;
                auto dz = far - near;
                return Matrix4(2.f / dx,
                               0.f,
                               0.f,
                               0.f,
                               0.f,
                               2.f / dy,
                               0.f,
                               0.f,
                               0.f,
                               0.f,
                               2.f / dz,
                               0.f,
                               -(right + left) / dx,
                               -(top + bottom) / dy,
                               -(far + near) / dz,
                               1.f);
            }

            T& Value(int32_t col, int32_t row) { return values_[Index(col, row)]; }
            T Value(int32_t col, int32_t row) const { return values_[Index(col, row)]; }
            T operator[](const int32_t idx) const { return values_[idx]; }
            const std::array<T, 16>& values() const { return values_; }
           private:
            static inline int32_t Index(int32_t col, int32_t row) { return col * 4 + row; }
            std::array<T, 16> values_;
        };
    }
}

#endif
