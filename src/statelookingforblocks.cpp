#include "statelookingforblocks.h"


stateLookingForBlocks::stateLookingForBlocks(states *previousState):states(previousState)
{
    name = "State looking for cubes";
}

void stateLookingForBlocks::processData(){
    startProcessData();
        wallFollow();

    if(foundCube()){
        nextState=new stateGoingToCube(this);
    }
    finishProcessData();
}
