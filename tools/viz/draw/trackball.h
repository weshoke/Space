#ifndef SPACE_VIZ_DRAW_TRACKBALL_H
#define SPACE_VIZ_DRAW_TRACKBALL_H

#include "camera.h"

namespace viz {
    namespace draw {
        class Trackball {
           public:
            Trackball(float rot_speed)
            : rot_speed_(rot_speed)
            {
            }

            void Begin(const Vec2 &mouse) { mouse_ = mouse; }
            void Next(const Vec2 &mouse, Camera &camera)
            {
                auto move = mouse - mouse_;
                auto pan = camera.Pan() * move[0];
                auto up = camera.up() * move[1];
                auto axis = E3::CrossProduct(pan + up, camera.look_at()).Normalized();
                auto rot = E3::AxisAngle(axis, move.Norm());
                camera.Spin(rot);
                mouse_ = mouse;
            }

           private:
            Vec2 mouse_;
            float rot_speed_;
        };
    }
}

auto move_direction = move - prev_move;
auto angle = move_direction.Norm();
if (angle > 0.f) {
    auto eye_direction = camera.LookAt();
    auto up = camera.Up();
    auto pan_direction = E3f::CrossProduct(eye_direction, up);
    auto up_scale = move[1] - move_prev[1];
    auto pan_scale = move[0] - move_prev[0];
    auto axis = E3f::CrossProduct(up + pan_direction, eye_direction).Normalized();
    auto rot = Ef3::AxisAngle(axis, angle * rot_speed_);
    auto eye = eye.Spin(rot);
    up = eye.Spin(rot);
}

// 2d move
// moveDirection.set( _moveCurr.x - _movePrev.x, _moveCurr.y - _movePrev.y, 0 );
// angle = moveDirection.length();
//
// if ( angle ) {
//
// 	_eye.copy( _this.object.position ).sub( _this.target );
//
// 	eyeDirection.copy( _eye ).normalize();
// 	objectUpDirection.copy( _this.object.up ).normalize();
// 	objectSidewaysDirection.crossVectors( objectUpDirection, eyeDirection ).normalize();
//
// 	objectUpDirection.setLength( _moveCurr.y - _movePrev.y );
// 	objectSidewaysDirection.setLength( _moveCurr.x - _movePrev.x );
//
// 	moveDirection.copy( objectUpDirection.add( objectSidewaysDirection ) );
//
// 	axis.crossVectors( moveDirection, _eye ).normalize();
//
// 	angle *= _this.rotateSpeed;
// 	quaternion.setFromAxisAngle( axis, angle );
//
// 	_eye.applyQuaternion( quaternion );
// 	_this.object.up.applyQuaternion( quaternion );
//
// 	_lastAxis.copy( axis );
// 	_lastAngle = angle;
//
// } else if ( ! _this.staticMoving && _lastAngle ) {
//
// 	_lastAngle *= Math.sqrt( 1.0 - _this.dynamicDampingFactor );
// 	_eye.copy( _this.object.position ).sub( _this.target );
// 	quaternion.setFromAxisAngle( _lastAxis, _lastAngle );
// 	_eye.applyQuaternion( quaternion );
// 	_this.object.up.applyQuaternion( quaternion );
//
// }
//
// _movePrev.copy( _moveCurr );

namespace viz {
    namespace draw {
    }
}

#endif
