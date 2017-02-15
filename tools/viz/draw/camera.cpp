#include "camera.h"
#include "entry/cmd.h"
#include "entry/entry.h"
#include "entry/input.h"
#include <bx/allocator.h>
#include <bx/fpumath.h>
#include <bx/timer.h>
#include <iostream>

namespace {
    int CmdMove(CmdContext* /*_context*/, void* ud, int argc, char const* const* argv)
    {
        if (argc > 1) {
            auto camera = static_cast<space::draw::Camera*>(ud);
            if (0 == strcmp(argv[1], "forward")) {
                camera->SetKeyState(CAMERA_KEY_FORWARD, true);
                return 0;
            }
            else if (0 == strcmp(argv[1], "left")) {
                camera->SetKeyState(CAMERA_KEY_LEFT, true);
                return 0;
            }
            else if (0 == strcmp(argv[1], "right")) {
                camera->SetKeyState(CAMERA_KEY_RIGHT, true);
                return 0;
            }
            else if (0 == strcmp(argv[1], "backward")) {
                camera->SetKeyState(CAMERA_KEY_BACKWARD, true);
                return 0;
            }
            else if (0 == strcmp(argv[1], "up")) {
                camera->SetKeyState(CAMERA_KEY_UP, true);
                return 0;
            }
            else if (0 == strcmp(argv[1], "down")) {
                camera->SetKeyState(CAMERA_KEY_DOWN, true);
                return 0;
            }
        }

        return 1;
    }

    static void Cmd(const void* _userData) { cmdExec((const char*)_userData); }
    static const InputBinding s_cam_bindings[] = {
        {entry::Key::KeyW, entry::Modifier::None, 0, Cmd, "move forward"},
        {entry::Key::KeyA, entry::Modifier::None, 0, Cmd, "move left"},
        {entry::Key::KeyS, entry::Modifier::None, 0, Cmd, "move backward"},
        {entry::Key::KeyD, entry::Modifier::None, 0, Cmd, "move right"},
        {entry::Key::KeyQ, entry::Modifier::None, 0, Cmd, "move down"},
        {entry::Key::KeyE, entry::Modifier::None, 0, Cmd, "move up"},
        INPUT_BINDING_END};
}

namespace space {
    namespace draw {
        Camera::Camera()
        : eye_(0.f, 0.f, -35.f)
        , at_(0.f, 0.f, -1.f)
        , up_(0.f, 1.f, 0.f)
        , horizontal_angle_(0.01f)
        , vertical_angle_(0.f)
        , mouse_speed_(0.0020f)
        , move_speed_(30.f)
        , keys_(0)
        , mouse_down_(false)
        {
            Reset();
            entry::MouseState mouse_state;
            Update(0.0f, mouse_state);

            cmdAdd("move", CmdMove, this);
            inputAddBindings("cam_bindings", s_cam_bindings);
        }

        Camera::~Camera() { inputRemoveBindings("cam_bindings"); }
        void Camera::Reset()
        {
            m_mouseNow.m_mx = 0;
            m_mouseNow.m_my = 0;
            m_mouseLast.m_mx = 0;
            m_mouseLast.m_my = 0;
        }

        void Camera::SetKeyState(uint8_t _key, bool _down)
        {
            keys_ &= ~_key;
            keys_ |= _down ? _key : 0;
        }

        static bool SimulatedRightButton()
        {
            return inputGetModifiersState() &&
                   (entry::Modifier::LeftMeta | entry::Modifier::RightMeta);
        }

        void Camera::Update(float _deltaTime, const entry::MouseState& _mouseState)
        {
            if (!mouse_down_) {
                m_mouseLast.m_mx = _mouseState.m_mx;
                m_mouseLast.m_my = _mouseState.m_my;
            }

            // if(inputGetModifiersState() != 0)
            // std::cout << "Mod: " << int(inputGetModifiersState()) << "\n";

            mouse_down_ = !!_mouseState.m_buttons[entry::MouseButton::Right];
            static int count = 0;
            if (!mouse_down_ && SimulatedRightButton()) {
                ++count;
                if (count >= 10) {
                    count = 0;
                }
                // std::cout << count << " SiM: " <<
                // !!_mouseState.m_buttons[entry::MouseButton::Left] << "\n";
                // mouse_down_ = !!_mouseState.m_buttons[entry::MouseButton::Left];
            }

            if (mouse_down_) {
                m_mouseNow.m_mx = _mouseState.m_mx;
                m_mouseNow.m_my = _mouseState.m_my;
            }

            if (mouse_down_) {
                int32_t deltaX = m_mouseNow.m_mx - m_mouseLast.m_mx;
                int32_t deltaY = m_mouseNow.m_my - m_mouseLast.m_my;

                horizontal_angle_ += mouse_speed_ * float(deltaX);
                vertical_angle_ -= mouse_speed_ * float(deltaY);

                m_mouseLast.m_mx = m_mouseNow.m_mx;
                m_mouseLast.m_my = m_mouseNow.m_my;
            }

            float direction[3] = {
                cosf(vertical_angle_) * sinf(horizontal_angle_),
                sinf(vertical_angle_),
                cosf(vertical_angle_) * cosf(horizontal_angle_),
            };

            float right[3] = {
                sinf(horizontal_angle_ - bx::piHalf), 0, cosf(horizontal_angle_ - bx::piHalf),
            };

            float up[3];
            bx::vec3Cross(up, right, direction);

            if (keys_ & CAMERA_KEY_FORWARD) {
                float pos[3];
                bx::vec3Move(pos, eye_.Data());

                float tmp[3];
                bx::vec3Mul(tmp, direction, _deltaTime * move_speed_);

                bx::vec3Add(eye_.Data(), pos, tmp);
                SetKeyState(CAMERA_KEY_FORWARD, false);
            }

            if (keys_ & CAMERA_KEY_BACKWARD) {
                float pos[3];
                bx::vec3Move(pos, eye_.Data());

                float tmp[3];
                bx::vec3Mul(tmp, direction, _deltaTime * move_speed_);

                bx::vec3Sub(eye_.Data(), pos, tmp);
                SetKeyState(CAMERA_KEY_BACKWARD, false);
            }

            if (keys_ & CAMERA_KEY_LEFT) {
                float pos[3];
                bx::vec3Move(pos, eye_.Data());

                float tmp[3];
                bx::vec3Mul(tmp, right, _deltaTime * move_speed_);

                bx::vec3Add(eye_.Data(), pos, tmp);
                SetKeyState(CAMERA_KEY_LEFT, false);
            }

            if (keys_ & CAMERA_KEY_RIGHT) {
                float pos[3];
                bx::vec3Move(pos, eye_.Data());

                float tmp[3];
                bx::vec3Mul(tmp, right, _deltaTime * move_speed_);

                bx::vec3Sub(eye_.Data(), pos, tmp);
                SetKeyState(CAMERA_KEY_RIGHT, false);
            }

            if (keys_ & CAMERA_KEY_UP) {
                float pos[3];
                bx::vec3Move(pos, eye_.Data());

                float tmp[3];
                bx::vec3Mul(tmp, up, _deltaTime * move_speed_);

                bx::vec3Add(eye_.Data(), pos, tmp);
                SetKeyState(CAMERA_KEY_UP, false);
            }

            if (keys_ & CAMERA_KEY_DOWN) {
                float pos[3];
                bx::vec3Move(pos, eye_.Data());

                float tmp[3];
                bx::vec3Mul(tmp, up, _deltaTime * move_speed_);

                bx::vec3Sub(eye_.Data(), pos, tmp);
                SetKeyState(CAMERA_KEY_DOWN, false);
            }

            bx::vec3Add(at_.Data(), eye_.Data(), direction);
            bx::vec3Cross(up_.Data(), right, direction);
        }

        void Camera::GetViewMtx(float* _viewMtx)
        {
            bx::mtxLookAt(_viewMtx, eye_.Data(), at_.Data(), up_.Data());
        }

        void Camera::SetPosition(const Vec3& pos) { eye_ = pos; }
        void Camera::SetHorizontalAngle(float horizontal_angle)
        {
            horizontal_angle_ = horizontal_angle;
        }

        void Camera::SetVerticalAngle(float vertical_angle) { vertical_angle_ = vertical_angle; }
    }
}
