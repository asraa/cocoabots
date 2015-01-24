#include "stateapproachblock.h"
#include "statecollectingcube.h"
stateApproachBlock::stateApproachBlock(states * previousState):states(states * previousState)
{
}

void stateApproachBlock::processData(){
    startProcessData();

    if(foundCube()){
        followPoint(getDistanceNearestCube(),getAngleNearestCube());
    }
    if(finishedFollowingPoint){
        nextState= new stateCollectingCube;
    }

    finishProcessData();
}
