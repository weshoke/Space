#ifndef SPACE_VIZ_DRAW_TRACKBALL_H
#define SPACE_VIZ_DRAW_TRACKBALL_H

#include "camera.h"

namespace viz {
    namespace draw {
        class Trackball {
           public:
            Trackball(float rot_speed = 1.f, float move_speed = 1.f, float pan_speed = 1.f)
            : camera_(Camera::Default())
            , rot_speed_(rot_speed)
            , move_speed_(move_speed)
            , pan_speed_(pan_speed)
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

            void Move(Camera &camera, float amt)
            {
                camera.Translate(camera.Look() * amt * move_speed());
            }

            Camera Pan(const Vec2 &mouse)
            {
                auto camera = camera_;
                auto move = (mouse - mouse_) * pan_speed();
                auto pan = camera.Pan() * move[0];
                auto up = camera.up() * move[1];
                camera.Translate(pan + up);
                return camera;
            }

            float rot_speed() const { return rot_speed_; }
            float move_speed() const { return move_speed_; }
            float pan_speed() const { return pan_speed_; }
           private:
            Camera camera_;
            Vec2 mouse_;
            float rot_speed_;
            float move_speed_;
            float pan_speed_;
        };
    }
}

#endif
