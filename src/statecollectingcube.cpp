#include "statecollectingcube.h"
#include "statelookingforblocks.h"
#include "stateapproachblock.h"
#include "stategoingtocube.h"

stateCollectingCube::stateCollectingCube(states *previousState, int color):states(previousState)
{
    name = "State Collecting Cube";
    myColor=color;
    cubeFound=0;
    myWaitTime=0;

}

void stateCollectingCube::processData(){
    startProcessData();

    collectBlock(myColor);
    if (finishedCollectingBlock){
        if(successfullyCollectedBlock){
            myWaitTime=getTimeMicroseconds()+COLLECTING_CUBE_WAIT_TIME_LATER_MS*1000;
            stop();
        }
        else{
            myWaitTime=0;
        }
        cubeFound=foundCube();
        cubeDistance = getDistanceNearestCube();
        if(cubeFound){
            if (cubeDistance< FOLLOW_POINT_DISTANCE_INCHES){
                nextState = new stateGoingToCube(this,GO_TO_CUBE_WAIT_AFTER_COLLECTING_MS);
            }else{
                nextState = new stateApproachBlock(this);
            }
        }
        else{
            nextState = new stateLookingForBlocks(this);
        }
    }
}

stateCollectingCube::~stateCollectingCube(){

}
