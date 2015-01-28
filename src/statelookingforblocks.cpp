#include "statelookingforblocks.h"
#include "stateapproachblock.h"

stateLookingForBlocks::stateLookingForBlocks(states *previousState):states(previousState)
{
    name = "State looking for cubes";
    startTimeState =getTimeMicroseconds();
    myPrivateState = wallFollowing;
}

void stateLookingForBlocks::processData(){
    startProcessData();
    switch(myPrivateState){
        case (wallFollowing):
            wallFollow();
            if(startTimeState > LOOKING_FOR_BLOCKS_STOP_AND_LOOK_TIME_MS){
                myPrivateState=looking;
            }
            break;
    case(looking):
        turnNDegreesSlowly(135);
        if(finishedTurningNDegreesSlowly){
            myPrivateState = comingBack;
        }
        break;
    case(comingBack):
        turnNDegreesSlowly(-135);
        if(finishedTurningNDegreesSlowly){
            myPrivateState = comingBack;
        }
        break;
    }

    if(foundCube()){
        nextState=new stateApproachBlock(this);
    }
    finishProcessData();
}
