#include "cocoabot.h"
#include <unistd.h>

cocoabot::cocoabot(): //Initializes all the modules
    mySensors(),
    myMotorControl(&mySensors),
    myServosControl(),
    myActuator(myMotorControl,myServosControl),
    myUtils()

{
    //Starts the state machine
    myState=new stateStart(&myMotorControl,
                           &myServosControl,
                           &mySensors,
                           &myUtils);
}

//Runs the state machine
void cocoabot::run(){
    while (running){
        myState->processData();
        states * nextState = myState->getNextState();
        if (nextState!=myState){
            delete myState;
            myState=nextState;
        }
        usleep(UPDATE_RATE_STATE_MACHINE_MICROSECONDS);
    }
}
