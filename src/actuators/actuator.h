#ifndef ACTUATOR_H
#define ACTUATOR_H
#include "motorwheel.h"
#include "pwmutils.h"
#include "../configFile.h"
#include "../sensorsmodule.h"

class actuator
{
public:
    actuator(sensorsModule * sensors);
    actuator();
    motorWheel rightWheel;
    motorWheel leftWheel;
    pwmUtils pwm;
    sensorsModule * sensorsPointer;

    //Speed = number from -1 to 1
    void setPowerRightWheel(double speed);
    void setPowerLeftWheel(double speed);
    void setAngleServo(double angle);
};

#endif // ACTUATOR_H
