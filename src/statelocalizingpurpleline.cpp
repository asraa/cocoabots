#include "statelocalizingpurpleline.h"

stateLocalizingPurpleLine::stateLocalizingPurpleLine(states *previousState):states(previousState)
{
    name = "State localizing purple line";

}

void stateLocalizingPurpleLine::processData(){
    startProcessData();

    finishProcessData();
}

stateLocalizingPurpleLine::~stateLocalizingPurpleLine(){

}
