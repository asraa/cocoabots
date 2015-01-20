#ifndef STATES_H
#define STATES_H
#include "actuators/motorscontrol.h"
#include "actuators/servoscontrol.h"
#include "sensorsmodule.h"
#include "configFile.h"
#include "utils.h"
class states
{
public:
    states(motorsControl * motorControlPointer,
           servosControl * servoControlPointer,
           sensorsModule * sensorsPointer,
           utils * utilsPointer);
    states(states * previouStatePointer);
    states * getNextState(); //Can return this, or a new state
    virtual void processData() = 0; //This is the brain of the robot
protected:
    //Functions that should always be called.
    void startProcessData(); //Should be called in the beginning of process data.
                            //Takes care of procedures that have state machines inside them.
    void finishProcesData(); // Should be called in the end of process data.
                            //Takes care of procedures that have state machines inside them.

    //Here are the helper functions for the states
    int getTimeRemainingGameSeconds();
    long long int getRunningTimeMicroSeconds();
    double getAngle();

    //Positive is defined to be to the right, i.e. a negative side distance will make it turn to the left
    double cartesianCoordinatesToAngle(double frontDistance, double sideDistance);
    volatile double getDistanceRightWall();
    volatile double getDistanceFrontWall();
    double getAngleDifference(double angle1, double angle2);
    void setCarrotPosition(double distance, double angle);

    //Here are the procedures that can be used in all states.
    //Many of them are implmented as state machines, so they should have two variables associated
    //So that they could be cleaned. Take as an example the wall follow procedure and startProcessData
    // and finish Process Data.

    //wallFollow(); Follow a wall on the right
    void wallFollow(); //procedure to wall follow
    int wallFollowed;  //Variable that says if we have wall followed on the current iteration of the state machine
    int wallFollowing; //Variable that says if we have been wall following between iterations of the state machine



    //Here are defined all the data that the states have acess to.
    long long startTimeStateMicroseconds;
    states * nextState;
    sensorsModule * mySensors;
    motorsControl * myMotorControl;
    servosControl * myServosControl;
    utils * myUtils;

};



#endif // STATES_H
