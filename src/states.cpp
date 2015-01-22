#include "states.h"
#include <cmath>
#include <cstdio>

states::states(motorsControl * motorControlPointer,
               servosControl * servoControlPointer,
               sensorsModule * sensorsPointer,
               ImageProcessor *imageProcessorPointer,
               utils *utilsPointer):
    myMotorControl(motorControlPointer),
    myServosControl(servoControlPointer),
    mySensors(sensorsPointer),
    myUtils(utilsPointer),
    myImageProcessor(imageProcessorPointer),
    nextState(this)
{
    startTimeStateMicroseconds = getTimeMicroseconds();
    wallFollowed=0;
    wallFollowing=0;
    finishedCollectingBlock=0;
    collectedBlocks=0;
    collectingBlocks=0;
    wentToPoint=0;
    goingToPoint=0;
    finishedGoingToPoint=0;
    name = "State Super Class";
}

states::states(states *previouStatePointer):states(
                                                previouStatePointer->myMotorControl,
                                                previouStatePointer->myServosControl,
                                                previouStatePointer->mySensors,
                                                previouStatePointer->myImageProcessor,
                                                previouStatePointer->myUtils){

}

std::string states::getName(){
    return name;
}


void states::wallFollow(){
    enum wallFollowState{lookingForWall, rotating, followingWall};
    static double initialTurningAngle =0;
    static wallFollowState myState;

    wallFollowed=1;
    if(!wallFollowing){
        myState = lookingForWall;
    }

    switch (myState) {
    case lookingForWall:

        if (getDistanceFrontWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
            myState = rotating;
            initialTurningAngle=getAngle();
            setCarrotPosition(0,-90);
            //printf("transitioning from looking for a wall to rotating\n");
        }
        else if (getDistanceRightWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
            //printf("transitioning from looking for a wall to following a wall");
            myState=followingWall;
        } else{
            setCarrotPosition(WALL_FOLLOW_CARROT_DISTANCE_INCHES,0);
            //printf("Im looking and my distance is %lf\n", getDistanceFrontWall());

        }
        break;

    case rotating:{
        double myAngle = getAngle();
        double angleDif =getAngleDifference(myAngle,initialTurningAngle);
        if (angleDif <-80){
            myState=followingWall;
            //printf("transitioning from rotating to following; myangle =%lf, initial angle = %lf, difference=%lf\n", myAngle, initialTurningAngle, angleDif);
        }
        break;
    }
    case followingWall:
        if (getDistanceFrontWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
            myState = rotating;
            initialTurningAngle=getAngle();
            setCarrotPosition(0,-90);
            //printf("transitioning from following for a wall to rotating\n");

        }
        else if (getDistanceRightWall()>WALL_FOLLOW_MAXIMUM_WALL_DISTANCE_INCHES){
            myState=lookingForWall;
            //printf("transitioning from following to  looking \n");


        }
        else{
            double carrotDistance = WALL_FOLLOW_CARROT_DISTANCE_INCHES;
            double carrotAngle;
            double wallDistance = getDistanceRightWall();
            double distanceToMoveToWall = wallDistance-WALL_FOLLOW_WALL_DISTANCE_INCHES;
            carrotAngle = cartesianCoordinatesToAngle(carrotDistance, distanceToMoveToWall);
            setCarrotPosition(carrotDistance,carrotAngle);

        }

        break;
    }

}

void states::collectBlock(int color){
    static long long int startTimeState;
    enum collectBlockState{resettingStart, moving, grabing,lifting, sorting,releasing, swipping, resettingFinish};
    static collectBlockState myState=resettingStart;
    collectedBlocks=1;

    if(!collectingBlocks){
        startTimeState = getTimeMicroseconds();
        myState=resettingStart;
    }

    long long int difTime;
    difTime=(getTimeMicroseconds()-startTimeState)/1000;
    switch(myState){
    case (resettingStart):
        if(difTime>BLOCK_COLLECT_RESET_TIME_MS){
            myState=moving;
            setCarrotPosition(BLOCK_COLLECT_DISTANCE_MOVE,0);
            startTimeState = getTimeMicroseconds();
        }
        else{
            myServosControl->reset();
        }
    case(moving):
        if(difTime>BLOCK_COLLECT_MAX_TIME_MOVING){
            myState=grabing;
            myServosControl->hookBlock();
            startTimeState = getTimeMicroseconds();
        }
    case(grabing): 
        if(difTime>BLOCK_COLLECT_GRAB_TIME_MS){
            myState=lifting;
            myServosControl->raiseBlock();
            startTimeState = getTimeMicroseconds();
        }
    case(lifting):
        if(difTime>BLOCK_COLLECT_LIFT_TIME_MS){
            myState=sorting;
            if (color == ImageProcessor::CUBE_COLOR_GREEN)
                myServosControl->sortGreen();
            else{
                myServosControl->sortRed();
            }
            startTimeState = getTimeMicroseconds();
        }
    case(sorting):
        if(difTime>BLOCK_COLLECT_SORT_TIME_MS){
            myState=releasing;
            myServosControl->unHookBlock();
            startTimeState = getTimeMicroseconds();
        }

    case(releasing):
        if(difTime>BLOCK_COLLECT_RELEASE_TIME_MS){
            myState=swipping;
            myServosControl->swipe();
            startTimeState = getTimeMicroseconds();
        }
    case(swipping):
        if(difTime>BLOCK_COLLECT_SWIPE_TIME_MS){ //sweeping
            myState=resettingFinish;
            myServosControl->reset();
            startTimeState = getTimeMicroseconds();
        }
    case(resettingFinish):
        if(difTime>BLOCK_COLLECT_RESET_TIME_MS){
            myState=resettingFinish;
            myServosControl->reset();
            startTimeState = getTimeMicroseconds();
            finishedCollectingBlock=1;
            myState=resettingStart;
            collectingBlocks=0;
        }

    }



}

void states::goToPoint(double distance, double angle){
    wentToPoint=1;
    if(!goingToPoint){
        setCarrotPosition(distance,angle);
    }
    else{
        if(getDistanceToCarrot()==0){
            finishedGoingToPoint=1;
        }
    }
}


int states::getTimeRemainingGameSeconds(){
    return myUtils->gameTimeRemaining();
}


states * states::getNextState(){
    return nextState;
}

long long int states::getRunningTimeMicroSeconds(){
    return getTimeMicroseconds()-startTimeStateMicroseconds;
}


double states::getAngle(){
    if( USE_GIROSCOPE_FOR_ANGLE){
        return mySensors->gyroscopeAngle;
    }else{
        return mySensors->encoderAngle;
    }
}


double states::cartesianCoordinatesToAngle(double frontDistance, double sideDistance){
    return atan2(sideDistance,frontDistance) / PI *180;
}


volatile double states::getDistanceRightWall(){
    return mySensors->rightShortIRData;
}

volatile double states::getDistanceFrontWall(){
    return mySensors->frontShortIRData;
}



void states::setCarrotPosition(double distance, double angle){
    myMotorControl->setNewDesiredRelativePositionInRadialCordinates(distance, angle);
}

double states::getAngleDifference(double angle1, double angle2){
    return myMotorControl->getAngleError(angle1,angle2);
}

double states::getDistanceToCarrot(){
    return myMotorControl->getPositionError();
}

long long int states::getTimeMicroseconds(){
    return mySensors->timeMicrosecondsSinceEpoch;
}


void states::startProcessData(){
    wallFollowed=0;
    collectedBlocks=0;
    wentToPoint=0;
}

int states::foundCube(){
    return myImageProcessor->foundCube;
}

void states::finishProcesData(){
    if (wallFollowed){
        wallFollowing=1;
    }
    else{
        wallFollowing=0;
    }

    if (collectedBlocks){
        collectingBlocks=1;
    }
    else{
        collectingBlocks=0;
    }

    if(wentToPoint){
        goingToPoint=1;
    }else{
        goingToPoint=0;
    }
}


states::~states(){

}
