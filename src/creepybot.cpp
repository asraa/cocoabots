#include "creepybot.h"
#include <cstring>
#include <unistd.h>
#include <cstdio>
#include "statetestprocedure.h"
#include <signal.h>
//#include "statelookingforpurpleline.h"

actuator * actPointerMain;
creepybot *cocoabotPointer;

//Global function that assures that we stop the motors when stopping the robot.
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

creepybot::~creepybot(){
    running=0;
    runThread->join();
    delete runThread;
}

creepybot::creepybot(): //Initializes all the modules
    mySensors(),
    myMotorControl(&mySensors),
    myServosControl(),
    myActuator(myMotorControl,myServosControl),
    myUtils(),
    myState(NULL)

{
    //Starts the state machine
    myState=new stateCreepy(&myMotorControl,
                           &myServosControl,
                           &mySensors,
                           &myUtils,
                           &myInstructions);
    cocoabotPointer =this;
    actPointerMain = &myActuator;
    signal(SIGINT, stopMotorsMain);
}

//Runs the state machine
void creepybot::start(){
    running=1;
    runThread = new std::thread(run,this);
}

void creepybot::run(void * creepybotPointer){
    creepybot * myCreepybot = (creepybot *)creepybotPointer;
    while (myCreepybot->running){
        if(myCreepybot->mySensors.onData){
            myCreepybot->myState->processData(&(myCreepybot->myInstructions));
        }
        else{
            myCreepybot->myState->stop();
        }
        usleep(UPDATE_RATE_STATE_MACHINE_MICROSECONDS);
    }
}



void creepybot::runTest(int argc, char **argv){

    if(strcmp(argv[1],"collectBlock")==0){
        while (running){

        }
    }

}

void creepybot::sendInstructions(float distance,
                                 float angle,
                                 int raiseArm,
                                 int sweep,
                                 int grab,
                                 float servoAngle1,
                                 float servoAngle2,
                                 int wallFollow){
    myInstructions.angle=angle;
    myInstructions.distance=distance;
    myInstructions.grab=grab;
    myInstructions.raiseArm=raiseArm;
    myInstructions.servoAngle1=servoAngle1;
    myInstructions.servoAngle2=servoAngle2;
    myInstructions.wallFollow=wallFollow;
    myInstructions.sweep=sweep;
    myInstructions.updated=true;
}
