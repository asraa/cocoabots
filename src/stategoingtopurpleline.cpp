#include "stategoingtopurpleline.h"

stateGoingToPurpleLine::stateGoingToPurpleLine(states *previousState):states(previousState)
{
}
void stateGoingToPurpleLine::processData(){
    static double distance;
    static double initialAngle;
    static double finalAngle;
    startProcessData();
    if((getTimeMicroseconds()-startTimeStateMicroseconds)/1000 <= GO_TO_CUBE_WAIT_TIME_MS){
        distance = getDistanceToPurpleLine(); //Function is states class
        initialAngle =  getInitialAngleToPurpleLine();
    }

    finishProcessData();
}
