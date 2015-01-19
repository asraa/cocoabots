#include "statewallfollow.h"

stateWallFollow::stateWallFollow(states *previousState):states(previousState)
{
}

void stateWallFollow::processData(){
    startProcessData();

    wallFollow();

    finishProcesData();
}
