#ifndef ACTUATOR_H
#define ACTUATOR_H
#include "motorwheel.h"
#include "pwmutils.h"
#include "../configFile.h"
#include "../sensorsmodule.h"
#include "servo.h"

class actuator
{
public:
    actuator(sensorsModule * sensors);
    actuator();
    motorWheel rightWheel;
    motorWheel leftWheel;
    pwmUtils pwm;
    servo sortServo;
    servo armServo;
    servo hookServo;
    sensorsModule * sensorsPointer;

    //Speed = number from -1 to 1
    void setPowerRightWheel(double speed);
    void setPowerLeftWheel(double speed);

    //Angle = number from 0 to 180 (preferrably not boundries? not sure.)
    void setSortServo(double angle);
    void setArmServo(double angle);
    void setHookServo(double angle);
};

#endif // ACTUATOR_H
