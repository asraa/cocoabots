#include "statelookingforpurpleline.h"
#include "stategoingtopurpleline.h"

stateLookingForPurpleLine::stateLookingForPurpleLine(states *previousState):states(previousState)
{
    name = "State looking for purple line";
    startTimeState =getTimeMicroseconds();
}

void stateLookingForPurpleLine::processData(){
    startProcessData();

    if (foundPurpleLine()){ // If purple line found:
        stop();
        int areWeInside=1; //TODO: LOOK AROUND
        nextState=new stateGoingToPurpleLine(this, areWeInside);
    }
    else{
        wallFollowLeftFast();
    }

    finishProcessData();
}
