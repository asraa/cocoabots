#include "statecollectingcube.h"
#include "statelookingforblocks.h"
#include "stateapproachblock.h"
#include "stategoingtocube.h"

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
        if(cubeFound){
            if (cubeDistance< FOLLOW_POINT_DISTANCE_INCHES){
                nextState = new stateGoingToCube(this);
            }else{
                nextState = new stateApproachBlock(this);
            }
        }
        else{
            nextState = new stateLookingForBlocks(this);
        }
    }
    cubeFound=foundCube();
    cubeDistance = getDistanceNearestCube();
    finishProcessData();
}

stateCollectingCube::~stateCollectingCube(){

}
