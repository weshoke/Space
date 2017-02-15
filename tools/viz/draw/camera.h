#ifndef SPACE_VIZ_DRAW_CAMERA_H
#define SPACE_VIZ_DRAW_CAMERA_H

#include "draw.h"
#include "entry/entry.h"

#define CAMERA_KEY_FORWARD UINT8_C(0x01)
#define CAMERA_KEY_BACKWARD UINT8_C(0x02)
#define CAMERA_KEY_LEFT UINT8_C(0x04)
#define CAMERA_KEY_RIGHT UINT8_C(0x08)
#define CAMERA_KEY_UP UINT8_C(0x10)
#define CAMERA_KEY_DOWN UINT8_C(0x20)

namespace space {
    namespace draw {
        class Camera {
           public:
            struct MouseCoords {
                int32_t m_mx;
                int32_t m_my;
            };

            Camera();
            ~Camera();
            void Reset();
            void SetKeyState(uint8_t _key, bool _down);
            void Update(float _deltaTime, const entry::MouseState& _mouseState);
            void GetViewMtx(float* _viewMtx);
            void SetPosition(const Vec3& pos);
            void SetHorizontalAngle(float horizontal_angle);
            void SetVerticalAngle(float vertical_angle);

            MouseCoords m_mouseNow;
            MouseCoords m_mouseLast;

            Vec3 eye_;
            Vec3 at_;
            Vec3 up_;
            float horizontal_angle_;
            float vertical_angle_;

            float mouse_speed_;
            float move_speed_;

            uint8_t keys_;
            bool mouse_down_;
        };
    }
}
#endif
