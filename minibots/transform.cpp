#include "transform.h"

Transform::Transform(bool invert_left_axis, bool invert_right_axis) {
    this->invert_left = invert_left_axis;
    this->invert_right = invert_right_axis;
}

int Transform::map_range(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Transform::transform(int left_y, int right_x, int &left_motor, int &right_motor) {
    // Arcade drive style transform
    int throttle = map_range(left_y, 0, 255, -127, 127);
    int turn = map_range(right_x, 0, 255, -127, 127);

    if (abs(throttle) < L_STICK_DEADZONE) throttle = 0;
    if (abs(turn) < R_STICK_DEADZONE) turn = 0;

    int left = throttle + turn;
    int right = throttle - turn;

    if (invert_left) {
        left = -left;
    }
    if (invert_right) {
        right = -right;
    }

    left_motor = map_range(left, -127, 127, MOTOR_MIN, MOTOR_MAX);
    right_motor = map_range(right, -127, 127, MOTOR_MIN, MOTOR_MAX);

    if (left_motor > MOTOR_MAX) left_motor = MOTOR_MAX;
    if (left_motor < MOTOR_MIN) left_motor = MOTOR_MIN;
    if (right_motor > MOTOR_MAX) right_motor = MOTOR_MAX;
    if (right_motor < MOTOR_MIN) right_motor = MOTOR_MIN;
}
