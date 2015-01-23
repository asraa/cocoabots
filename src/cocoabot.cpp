#include "cocoabot.h"
#include <cstring>
#include <unistd.h>
#include "statecollectingcube.h"
#include "statetestprocedure.h"

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
        states * nextState = new stateCollectingCube(myState,0);
        logger::log();
        previousState=myState;
        myState = nextState;
        delete previousState;
        previousState=NULL;

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

    if(strcmp(argv[1],"goToBlockAndCollect")==0){
        states * nextState = new stateTestProcedure(myState);
        logger::log();
        previousState=myState;
        myState = nextState;
        delete previousState;
        previousState=NULL;
        double cubePosition;
        double cubeAngle;
        int cubeColor;
        int answer;
        while (running){
            if (myState->foundCube()){
                cubePosition=myState->getDistanceNearestCube();
                cubeAngle = myState->getAngleNearestCube();
                cubeColor =myState->getColorNearestCube();
                printf("I found a cube at %lf in, %lf, degrees.\n It's color is %d\n' Go to cube? 1, 0\n", cubePosition,cubeAngle, cubeColor);
                scanf("%d", &answer);
                if (answer){
                    while ((running) && (!myState->finishedGoingToPoint)){
                        myState->startProcessingProceduresManual();
                        myState->goToPoint(cubePosition,cubeAngle);
                        myState->finishProcessingProceduresManual();
                    }
                    printf("Collect Cube? Green=1, Red=2 No= 0\n");
                    scanf("%d", &answer);
                    if (answer){
                        answer--;
                        while ((running) && (!myState->finishedCollectingBlock)){
                            myState->startProcessingProceduresManual();
                            myState->collectBlock(answer);
                            myState->finishProcessingProceduresManual();
                        }
                    }
                }
            }
        }
    }



}
