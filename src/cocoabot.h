#ifndef COCOABOT_H
#define COCOABOT_H
#include "states.h"
#include "statestart.h"
#include "sensorsmodule.h"
#include "actuators/motorscontrol.h"
#include "actuators/servoscontrol.h"
#include "actuators/actuator.h"
#include "utils.h"

class cocoabot
{
public:
    cocoabot();
    void run();
    int running;
private:
    sensorsModule mySensors;
    motorsControl myMotorControl;
    servosControl myServosControl;
    actuator myActuator;
    utils myUtils;
    states * myState;
};

#endif // COCOABOT_H
