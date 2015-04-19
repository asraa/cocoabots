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
    name= "State Start";
}

stateStart::stateStart(states *previouState):states(previouState){
    name= "State Start";
}

void stateStart::processData(){
    startProcessData();
    //do stuff

    //nextState = new stateWallFollow(this);
    nextState = new stateLookingForBlocks(this);
    //finish doing stuff
    finishProcessData();
}
