#include "statecollectingcube.h"
#include "statelookingforblocks.h"

stateCollectingCube::stateCollectingCube(states *previousState, int color):states(previousState)
{
    name = "State Collecting Cube";
    myColor=color;
}

void stateCollectingCube::processData(){
    startProcessData();

    collectBlock(myColor); //THIS IS WRONG. THIS IS FOR THE MOCK COMPETITION

    if (finishedCollectingBlock){
        nextState = new stateLookingForBlocks(this);
    }
    finishProcessData();
}
