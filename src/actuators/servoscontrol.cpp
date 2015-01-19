#include "servoscontrol.h"
#include <unistd.h>

servosControl::servosControl()
{
    armAngle = ARM_START;
    hookAngle = HOOK_START;
    sortAngle = SORT_START; //initialize this in the center

    this->running=1;
    runThread = new std::thread(run,this);
}

void servosControl::run(servosControl *myservo){
    while(myservo->running){
        usleep(UPDATE_RATE_SERVOS_MILISECONDS*1000);
        if (startCollect){
            static void timeStart = 0;

        }
        myservo->computeNewServosAngles();
    }
}

void servosControl::hookBlock(){
    hookAngle = HOOK_START + 80;
}

void servosControl::raiseBlock(){
    armAngle = ARM_START + 135;
}

void servosControl::sortRed(){
    hookAngle = HOOK_START;
    sortAngle = 135;
}

void servosControl::sortGreen(){
    hookAngle = HOOK_START;,,,,,,,,,,,,,,,,,,,,,,
    sortAngle = 45;
}

void servosControl::reset(){
    hookAngle = HOOK_START;
    armAngle = ARM_START;
    sortAngle = SORT_START;
}

servosControl::~servosControl(){
    running=0;
    runThread->join();
    delete runThread;
}
void servosControl::computeNewServosAngles(){
    /*if (startCollect){
                                                                 hookBlock();
        }
        if (time > TIME_FOR_HOOK )
    }
    else {

    }*/
}
