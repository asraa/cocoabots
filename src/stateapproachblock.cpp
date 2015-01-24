#include "stateapproachblock.h"
#include "statecollectingcube.h"
#include "stategoingtocube.h"
stateApproachBlock::stateApproachBlock(states * previousState):states(previousState)
{
}

void stateApproachBlock::processData(){
    startProcessData();

    if(foundCube()){
        followPoint(getDistanceNearestCube(),getAngleNearestCube());
    }
    if(finishedFollowingPoint){
        nextState= new stateGoingToCube(this);
    }

    finishProcessData();
}
