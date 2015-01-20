#include "statelookingforblocks.h"

stateLookingForBlocks::stateLookingForBlocks(states *previousState):states(previousState)
{
}

void stateLookingForBlocks::processData(){
    startProcessData();
        wallFollow();

    if(foundCube()){
        nextState=new stateGoingToCube(this);
    }
    finishProcesData();
}
