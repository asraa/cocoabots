#include "servoscontrol.h"
#include <unistd.h>

servosControl::servoscontrol()
{
    armAngle = 0;
    hookAngle = 0;
    sortAngle = 90; //initialize this in the center

    this->running=1;
    runThread = new std::thread(run,this);
}

void servosControl::run(servosControl *myservo){
    while(myservo->running){
        usleep(UPDATE_RATE_SERVOS_MILISECONDS*1000);
        myservo->computeNewServoAngles();
    }
}



void servosControl::hookBlock(){

}

void servosControl::raiseBlock(){

}

void servosControl::sortRed(){

}

void servosControl::sortGreen(){

}

servosControl::~servosControl(){
    running=0;
    runThread->join();
    delete runThread;
}
