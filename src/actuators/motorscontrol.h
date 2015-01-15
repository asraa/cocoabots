/* This class receives the desired angular speed of the robot and the desired speed of the robot
 * and assures that the robot stays at that speed. It runs its own thread that updates the
 * robot speed and sets the actuators to the desired power.
 *
 * This file depends on many other files. It depends on the robot having the encoders set up,
 * depends on the config file containing the maximum speed of each motor (revolutions per second),
 * maximum safe speed of the robot (maximum speed that the motors will be set to) and also maximum
 * angular speed.
 *
 * It makes use of a proportional controller to set the
 *
 * This might be a bad idea. I don't know other way to do this
 */


///TODO:
/// ADD MAXIMUM SAFE SPEED
#ifndef MOTORSCONTROL_H
#define MOTORSCONTROL_H
#include "../sensorsmodule.h"
#include "actuator.h"
#include"../configFile.h"
#define MICROSECOND 1000000
class motorsControl
{
public:
    motorsControl(sensorsModule * sensors);
    ~motorsControl();

    double desiredPosition;
    double desiredAngle;


    //double desiredNormalizedSpeed;
    //double desiredNormalizedAngularSpeed;
    //double desiredServoAngle;


    double realSpeed;
    double realAngularSpeed;

//    double normalizedSpeed;
//    double normalizedAngularSpeed;

    double previousAngle;
    double previousPosition;
    double previousTime;

    volatile double rightMotorPower;
    volatile double leftMotorPower;

    int running;
    std::thread *runThread;
    static void run(motorsControl * mycontrol);
    void computeNewMotorPowers();
    void updateSpeed();
    void updateAngularSpeed();
    void updateTime();
    void updatePosition();
    void updateAngle();
    double getNewAngle();
    double getNewPosition();

private:
    sensorsModule * mysensors;
};

#endif // MOTORSCONTROL_H
