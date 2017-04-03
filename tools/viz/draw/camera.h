#ifndef SPACE_VIZ_DRAW_CAMERA_H
#define SPACE_VIZ_DRAW_CAMERA_H

#include "geom/matrix4.h"
#include "primitives.h"

namespace viz {
    namespace draw {
        class Camera {
           public:
            enum class Mode { Projective = 0, Orthographic };

            Camera(const Vec3 &eye, const Vec3 &look_at, const Vec3 &up, float lens_angle)
            : Camera(eye, look_at, up, lens_angle, 1.f, 0.1f, 100.f)
            {
            }

            Camera(const Vec3 &eye,
                   const Vec3 &look_at,
                   const Vec3 &up,
                   float lens_angle,
                   float aspect)
            : Camera(eye, look_at, up, lens_angle, aspect, 0.1f, 100.f)
            {
            }

            Camera(const Vec3 &eye,
                   const Vec3 &look_at,
                   const Vec3 &up,
                   float lens_angle,
                   float aspect,
                   float near,
                   float far)
            : mode_(Mode::Projective)
            , eye_(eye)
            , look_at_(look_at)
            , up_(up)
            , lens_angle_(lens_angle)
            , aspect_(aspect)
            , near_(near)
            , far_(far)
            , left_(0.f)
            , right_(0.f)
            , bottom_(0.f)
            , top_(0.f)
            {
            }

            Camera(const Vec3 &eye,
                   const Vec3 &look_at,
                   const Vec3 &up,
                   float left,
                   float right,
                   float bottom,
                   float top,
                   float near,
                   float far)
            : mode_(Mode::Orthographic)
            , eye_(eye)
            , look_at_(look_at)
            , up_(up)
            , lens_angle_(0.f)
            , aspect_(0.f)
            , near_(near)
            , far_(far)
            , left_(left)
            , right_(right)
            , bottom_(bottom)
            , top_(top)
            {
            }

            static Camera Default(float aspect = 1.f)
            {
                return Camera(
                    Vec3(0.f, 0.f, -6.f), Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), 35.f, aspect);
            }

            static Camera Ortho(float left = -1.f,
                                float right = 1.f,
                                float bottom = -1.f,
                                float top = 1.f,
                                float near = -100.f,
                                float far = 100.f)
            {
                return Camera(Vec3(0.f, 0.f, 0.f),
                              Vec3(0.f, 0.f, 1.f),
                              Vec3(0.f, 1.f, 0.f),
                              left,
                              right,
                              bottom,
                              top,
                              near,
                              far);
            }

            Vec3 Look() const { return (look_at() - eye()).Normalized(); }
            Vec3 Pan() const { return E3::CrossProduct(up(), Look()); }
            template <class Rot>
            void Spin(const Rot &rot)
            {
                auto to_eye = eye() - look_at();
                eye_ = look_at() + to_eye.Spin(rot);
                up_ = up_.Spin(rot);
            }

            void Translate(const Vec3 &v)
            {
                eye_ = eye_ + v;
                look_at_ = look_at_ + v;
            }

            Matrix4 ViewMatrix() const { return Matrix4::LookAt(eye(), look_at(), up()); }
            Matrix4 ProjectionMatrix() const
            {
                if (mode() == Mode::Projective) {
                    return Matrix4::Perspective(lens_angle(), aspect(), near(), far());
                }
                return Matrix4::Ortho(left(), right(), bottom(), top(), near(), far());
            }

            Ray ViewRay(Vec3 p) { return Ray(eye(), (p - eye()).Normalized()); }
            Mode mode() const { return mode_; }
            const Vec3 &eye() const { return eye_; }
            const Vec3 &look_at() const { return look_at_; }
            const Vec3 &up() const { return up_; }
            float lens_angle() const { return lens_angle_; }
            float aspect() const { return aspect_; }
            float near() const { return near_; }
            float far() const { return far_; }
            float left() const { return left_; }
            float right() const { return right_; }
            float bottom() const { return bottom_; }
            float top() const { return top_; }
           protected:
            Mode mode_;
            Vec3 eye_;
            Vec3 look_at_;
            Vec3 up_;
            float lens_angle_;
            float aspect_;
            float near_;
            float far_;
            float left_;
            float right_;
            float bottom_;
            float top_;
        };
    }
}
#endif
