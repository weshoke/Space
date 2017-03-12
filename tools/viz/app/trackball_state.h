#ifndef SPACE_VIZ_APP_TRACKBALL_STATE_H
#define SPACE_VIZ_APP_TRACKBALL_STATE_H

#include "app.h"
#include "draw/camera.h"
#include "draw/trackball.h"
#include "mpark/variant.hpp"
#include <ostream>

namespace viz {
    namespace app {
        struct TrackballState {
           public:
            struct Idle;
            struct PanInit;
            struct Pan;
            struct RotateInit;
            struct Rotate;

            struct Idle {
                Idle(TrackballState& state)
                : state(state)
                {
                }

                void Mouse(int32_t button, int32_t action, int32_t mods)
                {
                    if (action == GLFW_PRESS) {
                        mods == GLFW_MOD_ALT ? state.Set<PanInit>() : state.Set<RotateInit>();
                    }
                }

                // No-op
                void Cursor(draw::Camera& camera,
                            draw::Trackball& trackball,
                            const draw::Vec2& mouse_pos)
                {
                }

                TrackballState& state;
            };

            struct PanInit {
                PanInit(TrackballState& state)
                : state(state)
                {
                }

                void Mouse(int32_t button, int32_t action, int32_t mods)
                {
                    state.IdleOnRelease(action);
                }

                void Cursor(draw::Camera& camera,
                            draw::Trackball& trackball,
                            const draw::Vec2& mouse_pos)
                {
                    trackball.Begin(camera, mouse_pos);
                    state.Set<Pan>();
                }

                TrackballState& state;
            };

            struct Pan {
                Pan(TrackballState& state)
                : state(state)
                {
                }

                void Mouse(int32_t button, int32_t action, int32_t mods)
                {
                    state.IdleOnRelease(action);
                }

                void Cursor(draw::Camera& camera,
                            draw::Trackball& trackball,
                            const draw::Vec2& mouse_pos)
                {
                    camera = trackball.Pan(mouse_pos);
                }

                TrackballState& state;
            };

            struct RotateInit {
                RotateInit(TrackballState& state)
                : state(state)
                {
                }

                void Mouse(int32_t button, int32_t action, int32_t mods)
                {
                    state.IdleOnRelease(action);
                }

                void Cursor(draw::Camera& camera,
                            draw::Trackball& trackball,
                            const draw::Vec2& mouse_pos)
                {
                    trackball.Begin(camera, mouse_pos);
                    state.Set<Rotate>();
                }

                TrackballState& state;
            };

            struct Rotate {
                Rotate(TrackballState& state)
                : state(state)
                {
                }

                void Mouse(int32_t button, int32_t action, int32_t mods)
                {
                    state.IdleOnRelease(action);
                }

                void Cursor(draw::Camera& camera,
                            draw::Trackball& trackball,
                            const draw::Vec2& mouse_pos)
                {
                    camera = trackball.Next(mouse_pos);
                }

                TrackballState& state;
            };

            using State = mpark::variant<Idle, PanInit, Pan, RotateInit, Rotate>;

            TrackballState()
            : state_(Idle(*this))
            {
            }

            template <class S>
            void Set()
            {
                state_.emplace<S>(*this);
            }

            void IdleOnRelease(int32_t action)
            {
                if (action == GLFW_RELEASE) {
                    Set<Idle>();
                }
            }

            void Mouse(int32_t button, int32_t action, int32_t mods)
            {
                switch (state_.index()) {
                    case 0:
                        mpark::get<0>(state_).Mouse(button, action, mods);
                        break;
                    case 1:
                        mpark::get<1>(state_).Mouse(button, action, mods);
                        break;
                    case 2:
                        mpark::get<2>(state_).Mouse(button, action, mods);
                        break;
                    case 3:
                        mpark::get<3>(state_).Mouse(button, action, mods);
                        break;
                    case 4:
                        mpark::get<4>(state_).Mouse(button, action, mods);
                        break;
                }
            }

            void Cursor(draw::Camera& camera,
                        draw::Trackball& trackball,
                        const draw::Vec2& mouse_pos)
            {
                switch (state_.index()) {
                    case 0:
                        mpark::get<0>(state_).Cursor(camera, trackball, mouse_pos);
                        break;
                    case 1:
                        mpark::get<1>(state_).Cursor(camera, trackball, mouse_pos);
                        break;
                    case 2:
                        mpark::get<2>(state_).Cursor(camera, trackball, mouse_pos);
                        break;
                    case 3:
                        mpark::get<3>(state_).Cursor(camera, trackball, mouse_pos);
                        break;
                    case 4:
                        mpark::get<4>(state_).Cursor(camera, trackball, mouse_pos);
                        break;
                }
            }

            friend std::ostream& operator<<(std::ostream& os, const TrackballState& state)
            {
                switch (state.state_.index()) {
                    case 0:
                        os << "Idle";
                        break;
                    case 1:
                        os << "PanInit";
                        break;
                    case 2:
                        os << "Pan";
                        break;
                    case 3:
                        os << "RotateInit";
                        break;
                    case 4:
                        os << "Rotate";
                        break;
                }
                return os;
            }

           private:
            State state_;
        };
    }
}

#endif
