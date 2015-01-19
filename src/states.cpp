#include "states.h"
#include <cmath>
states::states(motorsControl * motorControlPointer,
               servosControl * servoControlPointer,
               sensorsModule * sensorsPointer,
               utils *utilsPointer):
    myMotorControl(motorControlPointer),
    myServosControl(servoControlPointer),
    mySensors(sensorsPointer),
    myUtils(utilsPointer),
    nextState(this)
{
    startTimeStateMicroseconds = mySensors->timeMicrosecondsSinceEpoch;
    wallFollowed=0;
    wallFollowing=0;
}

states::states(states *previouStatePointer):states(
                                                previouStatePointer->myMotorControl,
                                                previouStatePointer->myServosControl,
                                                previouStatePointer->mySensors,
                                                previouStatePointer->myUtils){

}


void states::wallFollow(){
    wallFollowed=1; //Tells the state machine that we have wallFollowed

    //Those are state variables that keeps track of where we are in the procedure. They must be static
    static int foundWall=0; //If not, keep going forward until finding a wall
    static int turnedToWall=0; //If found wall, turn 90 degrees to the wall
    static int turning =0;
    static int wallInFrontOfRobot=0;
    static double initialTurningAngle =0; //Used to keep track if turned to the wall

    //Restart State Machine if we've just started wall following
    if (!wallFollowing){
        turning =0;
        foundWall=0;
        turnedToWall =0;
        initialTurningAngle = 0;
        wallInFrontOfRobot=0;
    }

    //If there is a wall in front of the robot, restart Turning
     if (getDistanceFrontWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
         if(!wallInFrontOfRobot){
             wallInFrontOfRobot=1;
             turning=0;
             turnedToWall=0;
             foundWall=1;
         }
         else{

         }
     }
     else{
         wallInFrontOfRobot=0;
     }
    //If already turned to the wall, follow the wall
    if (turnedToWall){
        double carrotDistance = WALL_FOLLOW_CARROT_DISTANCE_INCHES;
        double carrotAngle;
        double wallDistance = getDistanceRightWall();
        double distanceToMoveToWall = wallDistance-WALL_FOLLOW_WALL_DISTANCE_INCHES;
        carrotAngle = cartesianCoordinatesToAngle(carrotDistance, distanceToMoveToWall);
        setCarrotPosition(carrotDistance,carrotAngle);
    }
    //If wall is on the right, start following it
    else if (getDistanceRightWall() < WALL_FOLLOW_WALL_DISTANCE_INCHES){
        turnedToWall=1;
    }
    //If turned 80 degrees to the right, follow the wall
    else if(turning){
        if (getAngleDifference(initialTurningAngle,getAngle()) >80){
            turnedToWall=1;
        }
    }
    //If found wall, turn your right to it
    else if (foundWall){
        initialTurningAngle=getAngle();
        setCarrotPosition(0,-90);
        turning=1;
    }
    //If the wall is near, you found the wall
    else if (wallInFrontOfRobot){
        foundWall=1;
    }
    //Else, keep looking for a wall
    else{
        setCarrotPosition(WALL_FOLLOW_CARROT_DISTANCE_INCHES,0);
    }

}

int states::getTimeRemainingGameSeconds(){
    return myUtils->gameTimeRemaining();
}


states * states::getNextState(){
    return nextState;
}

long long int states::getRunningTimeMicroSeconds(){
    return mySensors->timeMicrosecondsSinceEpoch-startTimeStateMicroseconds;
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


double states::getDistanceRightWall(){
    return mySensors->rightShortIRData;
}

double states::getDistanceFrontWall(){
    return mySensors->frontShortIRData;
}

void states::setCarrotPosition(double distance, double angle){
    myMotorControl->setNewDesiredRelativePositionInRadialCordinates(distance, angle);
}

double states::getAngleDifference(double angle1, double angle2){
    return myMotorControl->getAngleError(angle1,angle2);
}

void states::startProcessData(){
    wallFollowed=0;
}


void states::finishProcesData(){
    if (wallFollowed){
        wallFollowing=1;
    }
    else{
        wallFollowing=0;
    }
}

