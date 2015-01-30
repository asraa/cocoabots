#include "statecollectingcube.h"
#include "statelookingforblocks.h"
#include "stateapproachblock.h"

stateCollectingCube::stateCollectingCube(states *previousState, int color):states(previousState)
{
    name = "State Collecting Cube";
    myColor=color;
    cubeFound=0;
}

void stateCollectingCube::processData(){
    startProcessData();

    collectBlock(myColor); //THIS IS WRONG. THIS IS FOR THE MOCK COMPETITION

    if (finishedCollectingBlock){
        if(cubeFound && cubeDistance< FOLLOW_POINT_DISTANCE_INCHES)
        nextState = new stateApproachBlock(this);
    }
    cubeFound=foundCube();
    cubeDistance = getDistanceNearestCube();
    finishProcessData();
}
