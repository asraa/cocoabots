#include "stategoingtocube.h"
#include "statecollectingcube.h"
#include "statelookingforblocks.h"


stateGoingToCube::stateGoingToCube(states *previousState):states(previousState)
{
    name= "State Going to Cube";
}

void stateGoingToCube::processData(){
    //Wait to get a still image
    if((getTimeMicroseconds()-startTimeStateMicroseconds)/1000 > GO_TO_CUBE_WAIT_TIME_MS){
        int cubeFound=foundCube();
        double distance = myImageProcessor->getNearestCubeDist()+GO_TO_CUBE_OVERSHOOT_DISTANCE;
        double angle = myImageProcessor->getNearestCubeAngle();
        int color = myImageProcessor->getNearestCubeColor();
        if(cubeFound){
            startProcessData();

            goToPoint(distance,angle);

            if (finishedGoingToPoint){
                nextState = new stateCollectingCube(this,color);
            }
        }
        else{
            nextState=new stateLookingForBlocks(this);
        }
        finishProcessData();
    }
}
