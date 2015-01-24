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
#include "../configFile.h"
#include  <thread>
#define MICROSECOND 1000000
class motorsControl
{
public:
    motorsControl(sensorsModule * sensors);
    ~motorsControl();

    //Set the desired Position and the desired angle. The run thread will automatically update
    //rightMotorPower and  leftMotorPower, that can be plugged into a actuator class

    double desiredPosition;
    double desiredAngle;

    //Those are the gains. They can be updated on the run, if necessary
    double fwdSpeedGain;
    double fwdErrorGain;
    double angSpeedGain;
    double angErrorGain;

    //This is the real Speed of the robot as seen by the encoders
    double realSpeed;
    //This is the real angular speed of the robot.
    double realAngularSpeed;

    //This is the normalized speed of the wheels. It is their speed divided by their maximum speed.
    double normalizedLeftWheelSpeed;
    double normalizedRightWheelSpeed;


    double rightMotorPower;
    double leftMotorPower;

    void setNewDesiredRelativePositionInRadialCordinates(double radiusInInches, double angle);

    int running;
    std::thread *runThread;
    static void run(motorsControl * mycontrol);
    void computeNewMotorPowers();

    int getAngleError(double myDesiredAngle, double realAngle);
    int getAngleError();
    double getPositionError(double desiredPosition, double realPosition);
    double getPositionError();
    double getNewAngle();
    double getNewAngleFromGyroscope();
    double getNewAngleFromEncoders();
    double getNewPosition();
    double getNewRightWheelPosition();
    double getNewLeftWheelPosition();

    double previousAngle;
    double previousPosition;
    double previousTime;
    double previousRightWheelPosition;
    double previousLeftWheelPosition;
private:
    int isTurning;
    double positionStartTurning;
    void updateTurningState(double angleError, double angleSpeed); //It disallows unsafeDynamicTurning;
    double ifTurningGetTurningAxisPosition(double nextPosition); //Assures that we turn in place
    void updateSpeed();
    void updateAngularSpeed();
    void updateTime();
    void updatePosition();
    void updateAngle();
    void updateWheelsSpeed();
    void updateWheelsPositions();
    double currentLimiter(double normalizedWheelSpeed, double power);
    double powerMinimumThreshold(double power);


    sensorsModule * mysensors;
};

#endif // MOTORSCONTROL_H
