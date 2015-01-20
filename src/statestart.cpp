#include "statestart.h"
#include "statewallfollow.h"
#include "statelookingforblocks.h"
stateStart::stateStart(motorsControl * motorControlPointer,
                       servosControl * servoControlPointer,
                       sensorsModule * sensorsPointer, ImageProcessor *imageProcessorPointer,
                       utils * utilsPointer):states(motorControlPointer,
                                                    servoControlPointer,
                                                    sensorsPointer,
                                                    imageProcessorPointer,
                                                    utilsPointer)
{
}

stateStart::stateStart(states *previouState):states(previouState){

}

void stateStart::processData(){
    startProcessData();
    //do stuff

    //nextState = new stateWallFollow(this);
    nextState = new stateLookingForBlocks(this);
    //finish doing stuff
    finishProcesData();
}
