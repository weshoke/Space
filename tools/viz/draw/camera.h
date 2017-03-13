#ifndef SPACE_VIZ_DRAW_CAMERA_H
#define SPACE_VIZ_DRAW_CAMERA_H

#include "geom/matrix4.h"
#include "primitives.h"
#include <iostream>

namespace viz {
    namespace draw {
        class Camera {
           public:
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
            : eye_(eye)
            , look_at_(look_at)
            , up_(up)
            , lens_angle_(lens_angle)
            , aspect_(aspect)
            , near_(near)
            , far_(far)
            {
            }

            static Camera Default(float aspect = 1.f)
            {
                return Camera(
                    Vec3(0.f, 0.f, -6.f), Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), 35.f, aspect);
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
                return Matrix4::Perspective(lens_angle(), aspect(), near(), far());
            }

            const Vec3 &eye() const { return eye_; }
            const Vec3 &look_at() const { return look_at_; }
            const Vec3 &up() const { return up_; }
            float lens_angle() const { return lens_angle_; }
            float aspect() const { return aspect_; }
            float near() const { return near_; }
            float far() const { return far_; }
           protected:
            Vec3 eye_;
            Vec3 look_at_;
            Vec3 up_;
            float lens_angle_;
            float aspect_;
            float near_;
            float far_;
        };
    }
}
#endif
