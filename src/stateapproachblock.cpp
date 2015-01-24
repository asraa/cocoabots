#include "stateapproachblock.h"
#include "statecollectingcube.h"
stateApproachBlock::stateApproachBlock(states * previousState):states(previousState)
{
}

void stateApproachBlock::processData(){
    startProcessData();

    if(foundCube()){
        followPoint(getDistanceNearestCube(),getAngleNearestCube());
    }
    if(finishedFollowingPoint){
        nextState= new stateCollectingCube(this,1);
    }

    finishProcessData();
}
