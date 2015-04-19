#include "stategoingtocube.h"
#include "statecollectingcube.h"
#include "statelookingforblocks.h"


stateGoingToCube::stateGoingToCube(states *previousState,
                                   long long int waitTimeMS):states(previousState)
{
    name= "State Going to Cube";
    mywaitTimeMS = waitTimeMS;
}

void stateGoingToCube::processData(){
    //Wait to get a still image
    static int cubeFound;
    static double distance;
    static double angle;
    static int color;
    startProcessData();

    if((getTimeMicroseconds()-startTimeStateMicroseconds)/1000 <= mywaitTimeMS){
        stop();
        cubeFound=foundCube();
        distance = getDistanceNearestCube()+GO_TO_CUBE_OVERSHOOT_DISTANCE;
        angle = getAngleNearestCube();
        color = getColorNearestCube();
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
