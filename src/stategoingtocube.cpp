#include "stategoingtocube.h"
#include "statecollectingcube.h"


stateGoingToCube::stateGoingToCube(states *previousState):states(previousState)
{
    name= "State Going to Cube";
}

void stateGoingToCube::processData(){
    static double distance = myImageProcessor->getNearestCubeDist();
    static double angle = myImageProcessor->getNearestCubeAngle();
    static int color = myImageProcessor->getNearestCubeColor();
    startProcessData();

    goToPoint(distance,angle);

    if (finishedGoingToPoint){
        nextState = new stateCollectingCube(this,color);
    }
    finishProcesData();
}
