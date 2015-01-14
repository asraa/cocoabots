#ifndef ACTUATOR_H
#define ACTUATOR_H
#include "motorwheel.h"
#include "pwmutils.h"
#include "../configFile.h"

class actuator
{
public:
    actuator();
    motorWheel rightWheel;
    motorWheel leftWheel;
    pwmUtils pwm;

    //Speed = number from -1 to 1
    void setPowerRightWheel(double speed);
    void setPowerLeftWheel(double speed);
    void setAngleServo(double angle);
};

#endif // ACTUATOR_H
