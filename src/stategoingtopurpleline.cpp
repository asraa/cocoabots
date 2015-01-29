#include "stategoingtopurpleline.h"

stateGoingToPurpleLine::stateGoingToPurpleLine(states *previousState):states(previousState)
{
}
void stateGoingToPurpleLine::processData(){
    startProcessData();
    finishProcessData();
}
