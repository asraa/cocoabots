#include "statewallfollow.h"


stateWallFollow::stateWallFollow(states *previousState):states(previousState)
{
    name = "State Wall Following";
}

void stateWallFollow::processData(){
    startProcessData();

    wallFollow();

    finishProcesData();
}
