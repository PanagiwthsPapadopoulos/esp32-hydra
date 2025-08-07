#ifndef JOYSTICK_H
#define JOYSTICK_H

enum class JoystickDirection {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    PRESS
};

void initJoystick();

void setDirection(JoystickDirection);
JoystickDirection getDirection();

void printJoystick();

void getJoystickDirection();


#endif // JOYSTICK_H