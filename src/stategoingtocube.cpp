#include "stategoingtocube.h"
#include "statecollectingcube.h"
#include "statelookingforblocks.h"


stateGoingToCube::stateGoingToCube(states *previousState,
                                   long long int waitTimeMS):states(previousState)
{
    name= "State Going to Cube";
    mywaitTimeMS = waitTimeMS;
    wallInFrontOfMe=0;
    if(getDistanceFrontWall()<GO_TO_CUBE_MINIMUM_DISTANCE_FRONT_WALL){
        wallInFrontOfMe=1;
    }
}

void stateGoingToCube::processData(){
    //Wait to get a still image
    static int cubeFound;
    static double distance;
    static double angle;
    static int color;
    startProcessData();


    if(wallInFrontOfMe){
        if((getTimeMicroseconds()-startTimeStateMicroseconds)/1000 <= GO_TO_CUBE_WALL_FOLLOW_TIME){
            wallFollow();
        }
        else{
            nextState = new stateLookingForBlocks(this);
        }
    }
    else{
        if((getTimeMicroseconds()-startTimeStateMicroseconds)/1000 <= mywaitTimeMS){
            stop();
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
}


stateGoingToCube::~stateGoingToCube(){

}
