#ifndef TRANSFORM_H
#define TRANSFORM_H

class Transform {
public:
    Transform(bool invert_left_axis, bool invert_right_axis);
    void transform(int left_y, int right_x, int &left_motor, int &right_motor);

private:
    bool invert_left;
    bool invert_right;

    int map_range(int x, int in_min, int in_max, int out_min, int out_max);

    static const int MOTOR_MIN = 0;
    static const int MOTOR_IDLE = 127;
    static const int MOTOR_MAX = 255;
    static const int L_STICK_DEADZONE = 10;
    static const int R_STICK_DEADZONE = 10;
};

#endif // TRANSFORM_H
