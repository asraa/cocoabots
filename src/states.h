#ifndef STATES_H
#define STATES_H
#include "actuators/motorscontrol.h"
#include "actuators/servoscontrol.h"
#include "sensorsmodule.h"
#include "utils.h"
class states
{
public:
    states(motorsControl * motorControlPointer,
           servosControl * servoControlPointer,
           sensorsModule * sensorsPointer,
           utils * utilsPointer);
    states * getNextState(); //Can return this, or a new
    virtual void processData() = 0; //This is the brain of the robot
private:
    long long startTimeStateMicroseconds;
    int getTimeRemainingGameSeconds();
    long long int getRunningTimeMicroSeconds();

    template <class newState>
    states * transitionToState(); // Use as: transitionToState<stateName>();

    states * nextState;
    sensorsModule * mySensors;
    motorsControl * myMotorControl;
    servosControl * myServosControl;
    utils * myUtils;

};

#endif // STATES_H
