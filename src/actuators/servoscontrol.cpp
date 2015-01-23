#include "servoscontrol.h"
#include <unistd.h>

servosControl::servosControl()
{
    armAngle = ARM_START;
    hookAngle = HOOK_START;
    sortAngle = SORT_START; //initialize this in the center
    previousSwipe=0;
    swipping =0;
    this->running=1;
    runThread = new std::thread(run,this);
}

void servosControl::run(servosControl *myservo){
    while(myservo->running){
        usleep(UPDATE_RATE_SERVOS_MILISECONDS*1000);
        myservo->computeNewServosAngles();
    }
}

void servosControl::hookBlock(){
    hookAngle = HOOK_START + 75;
}

void servosControl::unHookBlock(){
    hookAngle = HOOK_START;
}

void servosControl::raiseBlock(){
    armAngle = ARM_START + 146;
}

void servosControl::sortRed(){
    sortAngle = 135;
}

void servosControl::sortGreen(){
    sortAngle = 45;
}

void servosControl::reset(){
    hookAngle = HOOK_START;
    armAngle = ARM_START;
    sortAngle = SORT_START;
    swipping=0;
}

void servosControl::sweep(){
    swipping=1;
}

void servosControl::stopSweep(){
    swipping=0;
}

servosControl::~servosControl(){
    running=0;
    runThread->join();
    delete runThread;
}
void servosControl::computeNewServosAngles(){
    if (swipping){
        if (previousSwipe){
            sortGreen();
            previousSwipe=0;
        }
        else{
            sortRed();
            previousSwipe=1;
        }
        usleep(SWIPE_TIME_MS*1000);
    }

    /*if (startCollect){
                                                                 hookBlock();
        }
        if (time > TIME_FOR_HOOK )
    }
    else {

    }*/
}
