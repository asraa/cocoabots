#include "statelookingforpurpleline.h"
#include "stategoingtopurpleline.h"

stateLookingForPurpleLine::stateLookingForPurpleLine(states *previousState):states(previousState)
{
    name = "State looking for purple line";
    startTimeState =getTimeMicroseconds();
}

void stateLookingForPurpleLine::processData(){
    startProcessData();

    if (0){ // If purple line found:

        nextState=new stateGoingToPurpleLine(this);
    }
    else{
        wallFollowLeftFast();
    }

    finishProcessData();
}
