#ifndef SPACE_VIZ_DRAW_CAMERA_H
#define SPACE_VIZ_DRAW_CAMERA_H

#include "primitives.h"

namespace viz {
    namespace draw {
        class Camera {
           public:
            Camera(const Vec3 &eye, const Vec3 &look_at, const Vec3 &up, float lens_angle)
            : eye_(eye)
            , look_at_(look_at)
            , up_(up)
            , lens_angle_(lens_angle)
            {
            }

            static Camera Default()
            {
                return Camera(Vec3(0.f, 0.f, -5.f), Vec3f(0.f, 0.f, 1.f), Vec3(0.f, 1.f, 0.), 45.f);
            }

            Vec3 Pan() { return E3::CrossProduct(up(), look_at()); }
            template <class Rot>
            void Spin(const Rot &rot)
            {
                eye_ = eye_.Spin(rot);
                look_at_ = look_at_.Spin(rot);
                up_ = up_.Spin(rot);
            }

            const Vec3 &eye() const { return eye_; }
            const Vec3 &look_at() const { return look_at_; }
            const Vec3 &up() const { return up_; }
            float lens_angle() const { return lens_angle_; }
           protected:
            Vec3 eye_;
            Vec3 look_at_;
            Vec3 up_;
            float lens_angle_;
        };
    }
}
#endif
