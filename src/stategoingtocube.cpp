#include "stategoingtocube.h"
#include "statecollectingcube.h"
#include "statelookingforblocks.h"


stateGoingToCube::stateGoingToCube(states *previousState):states(previousState)
{
    name= "State Going to Cube";
}

void stateGoingToCube::processData(){
    //Wait to get a still image
    static int cubeFound;
    static double distance;
    static double angle;
    static int color;
    startProcessData();

    if((getTimeMicroseconds()-startTimeStateMicroseconds)/1000 <= GO_TO_CUBE_WAIT_TIME_MS){
        cubeFound=foundCube();
        distance = myImageProcessor->getNearestCubeDist()+GO_TO_CUBE_OVERSHOOT_DISTANCE;
        angle = myImageProcessor->getNearestCubeAngle();
        color = myImageProcessor->getNearestCubeColor();
    }
    else if(cubeFound){

        goToPoint(distance,angle);

        if (finishedGoingToPoint){
            nextState = new stateCollectingCube(this);//(this,color); //->pass camera color
                                                            //PASS NO COLOR PARAMETER TO GET DATA FROM THE COLOR SENSOR
        }
    }
    else{
        nextState=new stateLookingForBlocks(this);
    }
    finishProcessData();
}


stateGoingToCube::~stateGoingToCube(){

}
