#include "cocoabot.h"
#include <cstring>
#include <unistd.h>
#include "statecollectingcube.h"

actuator * actPointerMain;
cocoabot *cocoabotPointer;
void stopMotorsMain(int signo)
{
    if (signo == SIGINT) {
        cocoabotPointer->running=0;
        double a =0;
        actPointerMain->leftWheelPower = &a;
        actPointerMain->rightWheelPower =&a;
        actPointerMain->setPowerLeftWheel(0);
        actPointerMain->setPowerRightWheel(0);
    }
}

cocoabot::cocoabot(): //Initializes all the modules
    mySensors(),
    myMotorControl(&mySensors),
    myServosControl(),
    myActuator(myMotorControl,myServosControl),
    myUtils(),
    myImageProcessor(),
    myState(NULL),
    myLogger(&mySensors, &myMotorControl, &myServosControl, &myState, &myImageProcessor)

{
    //Starts the state machine
    myState=new stateStart(&myMotorControl,
                           &myServosControl,
                           &mySensors,
                           &myImageProcessor,
                           &myUtils);
    cocoabotPointer =this;
    actPointerMain = &myActuator;
    signal(SIGINT, stopMotorsMain);
}

//Runs the state machine
void cocoabot::run(){
    while (running){
        myState->processData();
        states * nextState = myState->getNextState();
        if (nextState!=myState){
            logger::log();
            previousState = myState;
            myState=nextState;
            delete previousState;
            previousState=NULL;
        }
        usleep(UPDATE_RATE_STATE_MACHINE_MICROSECONDS);
    }
}



void cocoabot::run(int argc, char **argv){
    if(strcmp(argv[1],"collectBlock")==0){
        states * nextState = new stateCollectingCube(myState);
        logger::log();
        previousState=myState;
        myState = nextState;
        delete previousState;
        previousState=NULL;

    }


    while (running){
        myState->processData();
        states * nextState = myState->getNextState();
        if (nextState!=myState){ //Makes sure we are always in the same state
            logger::log();
            previousState = nextState;
            delete previousState;
            previousState=NULL;
        }
        usleep(UPDATE_RATE_STATE_MACHINE_MICROSECONDS);
    }
}
