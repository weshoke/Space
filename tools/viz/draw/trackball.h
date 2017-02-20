#ifndef SPACE_VIZ_DRAW_TRACKBALL_H
#define SPACE_VIZ_DRAW_TRACKBALL_H

#include "camera.h"

namespace viz {
    namespace draw {
        class Trackball {
           public:
            Trackball(float rot_speed)
            : camera_(Camera::Default())
            , rot_speed_(rot_speed)
            {
            }

            void Begin(Camera &camera, const Vec2 &mouse)
            {
                camera_ = camera;
                mouse_ = mouse;
            }

            Camera Next(const Vec2 &mouse)
            {
                auto camera = camera_;
                auto look = camera.Look();
                auto move = (mouse - mouse_) * rot_speed();
                auto pan = camera.Pan() * move[0];
                auto up = camera.up() * move[1];
                auto axis = E3::CrossProduct(pan + up, look).Normalized();
                auto rot = E3::AxisAngle(axis, move.Norm());
                camera.Spin(rot);
                return camera;
            }

            float rot_speed() const { return rot_speed_; }
           private:
            Camera camera_;
            Vec2 mouse_;
            float rot_speed_;
        };
    }
}

#endif
