#include "statelookingforblocks.h"
#include "stateapproachblock.h"

stateLookingForBlocks::stateLookingForBlocks(states *previousState):states(previousState)
{
    name = "State looking for cubes";
}

void stateLookingForBlocks::processData(){
    startProcessData();
        wallFollow();

    if(foundCube()){
        nextState=new stateApproachBlock(this);
    }
    finishProcessData();
}
