#include "statestart.h"
#include "statewallfollow.h"
stateStart::stateStart(motorsControl * motorControlPointer,
                       servosControl * servoControlPointer,
                       sensorsModule * sensorsPointer,
                       utils * utilsPointer):states(motorControlPointer,
                                                    servoControlPointer,
                                                    sensorsPointer,
                                                    utilsPointer)
{
}

stateStart::stateStart(states *previouState):states(previouState){

}

void stateStart::processData(){
    startProcessData();
    //do stuff

    nextState = new stateWallFollow(this);
    //finish doing stuff
    finishProcesData();
}
