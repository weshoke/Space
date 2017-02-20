#ifndef SPACE_VIZ_DRAW_TRACKBALL_H
#define SPACE_VIZ_DRAW_TRACKBALL_H

#include "camera.h"

namespace viz {
    namespace draw {
        class Trackball {
           public:
            Trackball(float radius, float rot_speed)
            : radius_(radius)
            , rot_speed_(rot_speed)
            {
            }

            void Begin(const Vec2 &mouse) { mouse_ = mouse; }
            void Next(const Vec2 &mouse, Camera &camera)
            {
                auto move = mouse - mouse_;
                auto pan = camera.Pan() * move[0];
                auto up = camera.up() * move[1];
                auto axis = E3::CrossProduct(pan + up, camera.look()).Normalized();
                auto rot = E3::AxisAngle(axis, move.Norm());
                auto pos = camera.eye() + camera.look() * radius();
                camera.SpinAround(pos, rot);
                mouse_ = mouse;
            }

            float radius() const { return radius_; }
            float rot_speed() const { return rot_speed_; }
           private:
            Vec2 mouse_;
            float radius_;
            float rot_speed_;
        };
    }
}

#endif
