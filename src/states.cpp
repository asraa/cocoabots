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


//Complex procedures (Ones that have states and timeouts inside them)
void states::wallFollow(){
    enum wallFollowState{lookingForWall, rotating, followingWall};
    static long long int startTimeState;
    static double initialTurningAngle =0;
    static wallFollowState myState;
    long long int difTime;
    difTime=(getTimeMicroseconds()-startTimeState)/1000;

    wallFollowed=1;
    if(!wallFollowing){
        myState = lookingForWall;
        startTimeState=getTimeMicroseconds();
    }

    switch (myState) {
    case lookingForWall:

        if (getDistanceFrontWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
            if (getDistanceRightWall()<WALL_FOLLOW_MAXIMUM_WALL_DISTANCE_INCHES){
                myState = rotating;
                initialTurningAngle=getAngle();
                setCarrotPosition(0,-45);
                startTimeState = getTimeMicroseconds();

            }
            else{
                myState = rotating;
                initialTurningAngle=getAngle();
                setCarrotPosition(0,45);
                startTimeState = getTimeMicroseconds();

            }
            //printf("transitioning from looking for a wall to rotating\n");
        }
        else if (getDistanceRightWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
            //printf("transitioning from looking for a wall to following a wall");
            myState=followingWall;
        } else{
            if(difTime>WALL_FOLLOW_TIME_OUT_LOOKING_MS){
                setCarrotPosition(WALL_FOLLOW_CARROT_DISTANCE_INCHES,0);
            }
            else{
                sharpCurveToTheRight();
            }
            //printf("Im looking and my distance is %lf\n", getDistanceFrontWall());

        }
        break;

    case rotating:{
        double myAngle = getAngle();
        double angleDif =abs(getAngleToCarrot());
        if (angleDif <10 || difTime>WALL_FOLLOW_TIME_OUT_ROTATING_MS){
            myState=followingWall;
            startTimeState = getTimeMicroseconds();
            //printf("transitioning from rotating to following; myangle =%lf, initial angle = %lf, difference=%lf\n", myAngle, initialTurningAngle, angleDif);
        }
        break;
    }
    case followingWall:
        if (getDistanceFrontWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
            myState = rotating;
            initialTurningAngle=getAngle();
            setCarrotPosition(0,-45);
            startTimeState = getTimeMicroseconds();
            //printf("transitioning from following for a wall to rotating\n");

        }
        else if (getDistanceRightWall()>WALL_FOLLOW_MAXIMUM_WALL_DISTANCE_INCHES){
            myState=lookingForWall;
            startTimeState = getTimeMicroseconds();

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
        break;
    case(moving):
        if(difTime>BLOCK_COLLECT_MAX_TIME_MOVING){
            myState=grabing;
            myServosControl->hookBlock();
            startTimeState = getTimeMicroseconds();
        }
        break;
    case(grabing): 
        if(difTime>BLOCK_COLLECT_GRAB_TIME_MS){
            myState=lifting;
            myServosControl->raiseBlock();
            if (color == ImageProcessor::CUBE_COLOR_GREEN)
                myServosControl->sortGreen();
            else{
                myServosControl->sortRed();
            }
            startTimeState = getTimeMicroseconds();
        }
        break;
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
        break;
    case(sorting):
        if(difTime>BLOCK_COLLECT_SORT_TIME_MS){
            myState=releasing;
            myServosControl->unHookBlock();
            startTimeState = getTimeMicroseconds();
        }
        break;
    case(releasing):
        if(difTime>BLOCK_COLLECT_RELEASE_TIME_MS){
            myState=swipping;
            myServosControl->sweep();
            startTimeState = getTimeMicroseconds();
        }
        break;
    case(swipping):
        if(difTime>BLOCK_COLLECT_SWIPE_TIME_MS){ //sweeping
            myState=resettingFinish;
            myServosControl->reset();
            startTimeState = getTimeMicroseconds();
        }
        break;
    case(resettingFinish):
        if(difTime>BLOCK_COLLECT_RESET_TIME_MS){
            myState=resettingFinish;
            myServosControl->reset();
            startTimeState = getTimeMicroseconds();
            finishedCollectingBlock=1;
            myState=resettingStart;
            collectingBlocks=0;
            collectedBlocks=0;
        }
        break;

    }



}

void states::goToPoint(double distance, double angle){
    enum goToPointState{turning, going};
    static goToPointState myState=turning;
    static double myDistance=0;
    static double myAngle=0;
    double angleError;
    wentToPoint=1;
    if(!goingToPoint){
        finishedGoingToPoint=0;
        myState=turning;
        myAngle=angle;
        myDistance=distance;
        setCarrotPosition(0,myAngle);
    }
    switch(myState){
    case turning:
        angleError=getAngleToCarrot();
        if (angleError<=GO_TO_POINT_PRECISION_ANGLE&& -angleError<=GO_TO_POINT_PRECISION_ANGLE){
            setCarrotPosition(myDistance,angleError);
            myState=going;
        }
        break;
    case going:
        if(getDistanceToCarrot()<=GO_TO_POINT_PRECISION_INCHES){
            finishedGoingToPoint=1;
            wentToPoint=0;
        }
        break;
    }
}

void states::followPoint(double distance, double angle){
    followedPoint=1;
    if(!followingPoint){
        finishedFollowingPoint=0;
    }

    if(!finishedFollowingPoint){
        if (distance<FOLLOW_POINT_DISTANCE_INCHES){
            finishedFollowingPoint=1;
            followedPoint=0;
            setCarrotPosition(0,angle);
        }
        else{
            setCarrotPosition(FOLLOW_POINT_CARROT_DISTANCE,angle);
        }
    }
}

void states::curveToTheRight(){
    setCarrotPosition(CURVE_CARROT_DISTANCE,CURVE_CARROT_ANGLE);
}

void states::curveToTheLeft(){
    setCarrotPosition(CURVE_CARROT_DISTANCE,-CURVE_CARROT_ANGLE);
}

void states::mediumCurveToTheRight(){
    setCarrotPosition(MEDIUM_CURVE_CARROT_DISTANCE,CURVE_CARROT_ANGLE);
}

void states::mediumCurveToTheLeft(){
    setCarrotPosition(MEDIUM_CURVE_CARROT_DISTANCE,-CURVE_CARROT_ANGLE);
}


void states::sharpCurveToTheRight(){
    setCarrotPosition(SHARP_CURVE_CARROT_DISTANCE,SHARP_CURVE_CARROT_ANGLE);
}

void states::sharpCurveToTheLeft(){
    setCarrotPosition(SHARP_CURVE_CARROT_DISTANCE,-SHARP_CURVE_CARROT_ANGLE);
}

//Simple procedures (No states, no timeouts)


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

double states::getAngleToCarrot(){
    return myMotorControl->getAngleError();
}

long long int states::getTimeMicroseconds(){
    return mySensors->timeMicrosecondsSinceEpoch;
}

int states::foundCube(){
    return myImageProcessor->getFoundCube();
}

double states::getDistanceNearestCube(){
    return myImageProcessor->getNearestCubeDist();
}

double states::getAngleNearestCube(){
    return myImageProcessor->getNearestCubeAngle();
}

int states::getColorNearestCube(){
    return myImageProcessor->getNearestCubeColor();
}

void states::startProcessingProceduresManual(){
    startProcessData();;
}

void states::finishProcessingProceduresManual(){
    finishProcessData();
}

double states::abs(double number){
    if (number<0){
        number*=-1;
    }
    return number;
}

void states::startProcessData(){
    wallFollowed=0;
    collectedBlocks=0;
    wentToPoint=0;
    followedPoint=0;
}

void states::finishProcessData(){
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

    if (followedPoint)
        followingPoint=1;
    else
        followingPoint=0;
}


states::~states(){

}
