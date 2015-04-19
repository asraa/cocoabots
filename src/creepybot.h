#ifndef CREEPYBOT_H
#define CREEPYBOT_H
#include "states.h"
#include "creepyStates/statecreepy.h"
#include "sensorsmodule.h"
#include "actuators/motorscontrol.h"
#include "actuators/servoscontrol.h"
#include "actuators/actuator.h"
//#include "mapping/map.h"
//#include "localization/particlefilter.h"
#include "utils.h"
#include "creepyStates/instructions.h"

class creepybot
{
public:
    creepybot();
    void start();
    void runTest(int argc, char** argv);
    void sendInstructions( float distance,
                           float angle,
                           int raiseArm,
                           int sweep,
                           int grab,
                           float servoAngle1,
                           float servoAngle2,
                           int wallFollow);
    int running;
    std::thread *runThread;
    static void run(void * creepybotPointer);
    ~creepybot();
private:
    sensorsModule mySensors;
    motorsControl myMotorControl;
    servosControl myServosControl;
    actuator myActuator;
    utils myUtils;
    stateCreepy * myState;
    stateCreepy * previousState; //necessary for thread safety
    instructions myInstructions;
};

#endif // CREEPYBOT
