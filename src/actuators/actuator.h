#ifndef ACTUATOR_H
#define ACTUATOR_H
#include "motorwheel.h"
#include "pwmutils.h"
#include "../configFile.h"
#include "../sensorsmodule.h"
#include "motorscontrol.h"
#include "servo.h"

class actuator
{
public:
    //The actuator should receive an instance of sensors, so it can update the dir pin
    // of the encoder. //OBSOLETE
    actuator(sensorsModule * sensors);
    actuator();
    actuator(motorsControl &mymotorsControl);
    ~actuator();
    motorWheel rightWheel;
    motorWheel leftWheel;
    pwmUtils pwm;
    servo sortServo;
    servo armServo;
    servo hookServo;
    sensorsModule * sensorsPointer;

    static void run(actuator *myactuator);

    //Those points should point to the values that we desire to write to the motors
    int running;
    double * rightWheelPower;
    double * leftWheelPower;
    double sortServoAngle;
    double armServoAngle;
    double hookServoAngle;


    std::thread *runThread;
    //Speed = number from -1 to 1
    void setPowerRightWheel(double speed);
    void setPowerLeftWheel(double speed);

    //Angle = number from 0 to 180 (preferrably not boundries? not sure.)
    void setSortServo(double angle);
    double getSortServo();
    void setArmServo(double angle);
    double getArmServo();
    void setHookServo(double angle);
    double getHookServo();
};

#endif // ACTUATOR_H
