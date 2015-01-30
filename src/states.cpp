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

int states::myColorCount=0;
int states::theirColorCount=0;

std::string states::getName(){
    return name;
}


//Complex procedures (Ones that have states and timeouts inside them)


/*How to create a procedure with states and timeout.
 *
 * First:
 * Created three variables:
 * doneTheProcedure
 * doingTheProcedure
 * finishedDoingTheProcedure.
 *
 *Done the procedure means that we have done the procedure on this iteration of the state machine
 * Doing the procedure keeps track if we did the procedure on the previous iteration of the state machine.
 *
 * Edit on the state class the function: startProcessData and finishProcessData:
 * On startProcessData write:
 * doneTheProcedure = 0;
 *
 * setting Done the procedure to 0 means that we haven't done the procedure on this iteration of the state machine yet
 *
 * On the finishProcessData write:
 *
 * if (doneTheProcedure){
 *  doingTheProcedure =true;
 * }
 * else{
 *  doingTheProcedure = false;
 * }
 *
 *
 * Doing the procedure keeps track if we did the procedure on the previous iteration of the state machine.
 *
 * On the first lines of the procedure write:
 * doneTheProcedure=true;
 *
 * That is the way that we keep track of the procedure between iterations of the state machine.
 *
 *
 * Now, in the beginning of the procedure declare:
 *  --the enum state, that keeps track of the state of the procedure
 *  --the static variables, i.e. the variables that are kept
 *  between iteration. The most useful ones are listed here, but you might need others:
 * ///////////////////
 *
 * enum procedureState{procedureState1, procedureState2, procedureState3};
 * static procedureState myState;
 * static long long int startTimeState; //Start time of the state of this procedure; it is useful
 *                                      //for timeouts.
 * static double initialTurningAngle; //Useful for procedure that turn
 * static double initialPosition;  //Useful for procedures that move.
 *  //////////////
 *
 * Calculate useful differences such as, how much time has elapsed:
 *
 * //////////////////
 * long long int difTime;
 *   difTime=(getTimeMicroseconds()-startTimeState)/1000;
 *
 * /////////////////////
 *
 * Now write the code to reset the procedure to its start state, in case that
 * we are just restarting to do the procedure:
 *
 * /////////////////////////
 *     if(!doingTheProcedure){
 *       myState = procedureState1;
 *       startTimeState=getTimeMicroseconds();
 *   }
 * ////////////////////////////
 *
 *
 * And then, finally write the code. The state machine can realize actions during
 * the transitions between states, which is very useful for procedures dependent on time
 * Take a look at collectCube. All actions are realized on the transitions.
 *
 * Or it can realize actions during the state itself, which is useful for other types of conditions,
 * such as when wall following.
 *
 * The best is to make use of both.
 *
 *
 *
 *
 *
 * You should write:
 *
 * switch(myState){
 *
 * case(1)
 * (do stuff)
 *
 * break;
 * }
*/

void states::wallFollow(){
    wallFollowLeft();
}

//void states::wallFollowRight(){
//    enum wallFollowState{lookingForWall, rotating, followingWall};
//    static long long int startTimeState;
//    static double initialTurningAngle =0;
//    static wallFollowState myState;
//    long long int difTime;
//    difTime=(getTimeMicroseconds()-startTimeState)/1000;

//    wallFollowed=1;
//    if(!wallFollowing){
//        myState = lookingForWall;
//        startTimeState=getTimeMicroseconds();
//    }

//    switch (myState) {
//    case lookingForWall:

//        if (getDistanceFrontWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
//            if (getDistanceRightWall()<WALL_FOLLOW_MAXIMUM_WALL_DISTANCE_INCHES){
//                myState = rotating;
//                initialTurningAngle=getAngle();
//                setCarrotPosition(0,-45);
//                startTimeState = getTimeMicroseconds();

//            }
//            else{
//                myState = rotating;
//                initialTurningAngle=getAngle();
//                setCarrotPosition(0,45);
//                startTimeState = getTimeMicroseconds();

//            }
//            //printf("transitioning from looking for a wall to rotating\n");
//        }
//        else if (getDistanceRightWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
//            //printf("transitioning from looking for a wall to following a wall");
//            myState=followingWall;
//        } else{
//            if(difTime>WALL_FOLLOW_TIME_OUT_LOOKING_MS){
//            }
//            else{
//                sharpCurveToTheRight();
//            }
//            //printf("Im looking and my distance is %lf\n", getDistanceFrontWall());

//        }
//        break;

//    case rotating:{
//        double myAngle = getAngle();
//        double angleDif =abs(getAngleToCarrot());
//        if (angleDif <10 || difTime>WALL_FOLLOW_TIME_OUT_ROTATING_MS){
//            myState=followingWall;
//            startTimeState = getTimeMicroseconds();
//            //printf("transitioning from rotating to following; myangle =%lf, initial angle = %lf, difference=%lf\n", myAngle, initialTurningAngle, angleDif);
//        }
//        break;
//    }
//    case followingWall:
//        if (getDistanceFrontWall()<WALL_FOLLOW_WALL_DISTANCE_INCHES){
//            myState = rotating;
//            initialTurningAngle=getAngle();
//            setCarrotPosition(0,-45);
//            startTimeState = getTimeMicroseconds();
//            //printf("transitioning from following for a wall to rotating\n");

//        }
//        else if (getDistanceRightWall()>WALL_FOLLOW_MAXIMUM_WALL_DISTANCE_INCHES){
//            myState=lookingForWall;
//            startTimeState = getTimeMicroseconds();

//            //printf("transitioning from following to  looking \n");


//        }
//        else{
//            double carrotDistance = WALL_FOLLOW_CARROT_DISTANCE_INCHES;
//            double carrotAngle;
//            double wallDistance = getDistanceRightWall();
//            double distanceToMoveToWall = wallDistance-WALL_FOLLOW_WALL_DISTANCE_INCHES;
//            carrotAngle = cartesianCoordinatesToAngle(carrotDistance, distanceToMoveToWall);
//            setCarrotPosition(carrotDistance,carrotAngle);

//        }

//        break;
//    }

//}

void states::wallFollowRight(double carrotDistance,
                            double proportionalGain,
                            double wallDistance,
                            double maximumWallDistance,
                            double turningAngle,
                            double maxLookingForWallTime,
                            double wiggleTime,
                            double fastWiggleTime,
                            double maxTimeFollowingWall){
    enum wallFollowState{lookingForWall, rotating, followingWall};
    static long long int startTimeState;
    // EXPERIMENTAL ADDITION    // if left,front,right shortIRs are low,
    static int enteringATrap=0; // you need to turn around NOW!
    // EXPERIMENTAL ADDITION    // you're entering a trap!
    static int stuckOnACorner=0;
    static int wiggleDirection=0;
    static int wiggling=0;
    static int fastWiggling=0;
    static double initialTurningAngle =0;
    static int desiredAngle=0;
    static wallFollowState myState;
    long long int difTime;
    difTime=(getTimeMicroseconds()-startTimeState)/1000;

    wallFollowed=1;
    if(!wallFollowing){
        myState = lookingForWall;
        startTimeState=getTimeMicroseconds();
        wiggling=0;
        fastWiggling=0;
        stuckOnACorner=0;

        // EXPERIMENTAL ADDITION
        enteringATrap=0;
        // EXPERIMENTAL ADDITION
    }
    if(stuckOnACorner){
        //printf("Stuck on a corner \n");
        if (difTime<wiggleTime/2){
            setCarrotPosition(0,-50);
            return;

        }
        else if (difTime<wiggleTime*3.0/2){
                sharpCurveToTheRightBack();

        }
        else{
            startTimeState = getTimeMicroseconds();
            stuckOnACorner=0;
        }
    }
    else {
//        printf("Not stuck on a corner\n");

        if(getDistanceFrontWall()<WALL_FOLLOW_MINIMUM_DISTANCE_WALL && getDistanceRightWall()<WALL_FOLLOW_MINIMUM_DISTANCE_WALL){
            if (difTime>WALL_FOLLOW_MINIMUM_TIME_BEFORE_WIGGLE_MS){
                stuckOnACorner=1;
                startTimeState = getTimeMicroseconds();
            }
        }
//        if(enteringATrap){
//            if (difTime>WALL_FOLLOW_TRAP_TIME_MS || finishedTurningNDegreesQuickly){
//                startTimeState=getTimeMicroseconds();
//                enteringATrap=0;
//            }
//            else{
//                turnNDegreesQuickly(180);
//                //mediumCurveToTheLeft();
//            }
//            return;
//        }
        if(wiggling){
           // printf("Wiggling \n");
            if (difTime>wiggleTime){
                startTimeState=getTimeMicroseconds();//previousStartTimeState;
                wiggling=0;
                fastWiggling=0;

//                printf("Not wiggling anymore \n");
            }
            else{
                switch(wiggleDirection){
                case(0):
                    sharpCurveToTheRightBack();
                    break;
                case(1):
                    sharpCurveToTheLeftBack();
                    break;
                case(2):
                    sharpCurveToTheRightBack();
                    break;
                case(3):
                    sharpCurveToTheLeftBack();
                    break;
                }
            }
            return;
        }
        else if(fastWiggling){
            // printf("Wiggling \n");
             if (difTime>fastWiggleTime){
                 startTimeState=getTimeMicroseconds();//previousStartTimeState;
                 fastWiggling=0;
 //                printf("Not wiggling anymore \n");
             }
             else{
                 switch(wiggleDirection){
                 case(0):
                     sharpCurveToTheRightBack();
                     break;
                 case(1):
                     sharpCurveToTheLeftBack();
                     break;
                 case(2):
                     sharpCurveToTheRightBack();
                     break;
                 case(3):
                     sharpCurveToTheLeftBack();
                     break;
                 }
             }
             return;
         }
    }

    switch (myState) {
    case lookingForWall:

        if (getDistanceFrontWall()<wallDistance){
            if (getDistanceRightWall()<maximumWallDistance){
                myState = rotating;
                initialTurningAngle=getAngle();
                desiredAngle = turningAngle;

                //setCarrotPosition(0,turningAngle);
                startTimeState = getTimeMicroseconds();

            }
            else{

                myState = rotating;
                initialTurningAngle=getAngle();
                desiredAngle = turningAngle;

                //setCarrotPosition(0,turningAngle);
                startTimeState = getTimeMicroseconds();
            }
//            printf("transitioning from looking for a wall to rotating\n");
        }
        else if (getDistanceRightWall()<wallDistance){
            //printf("transitioning from looking for a wall to following a wall");
            myState=followingWall;
        } else{
            if(difTime>maxLookingForWallTime){
                //printf("starting to wiggle\n");
                //previousStartTimeState = startTimeState;
                startTimeState=getTimeMicroseconds();
                wiggling =true;
                wiggleDirection++;
                wiggleDirection%=4;
                stuckOnACorner=0;
                return;

            }
            else if(difTime>WALL_FOLLOW_TIME_OUT_LOOKING_MS){
                setCarrotPosition(carrotDistance,0);
            }
            else{
                sharpCurveToTheLeft();
                return;
            }
            //printf("Im looking and my distance is %lf\n", getDistanceFrontWall());

        }
        break;

    case rotating:{
        double myAngle = getAngle();
        double absAngleDif =abs(myAngle-initialTurningAngle);
        if (absAngleDif>abs(desiredAngle)||abs(absAngleDif-desiredAngle)<10|| difTime>WALL_FOLLOW_TIME_OUT_ROTATING_MS){
            myState=followingWall;
            startTimeState = getTimeMicroseconds();
            stop();
//            printf("transitioning from rotating to following; myangle =%lf, initial angle = %lf, difference=%lf\n", myAngle, initialTurningAngle, angleDif);
        }else{
            if(desiredAngle>0){
                turnToTheRightQuickly();
            }
            else{
                turnToTheLeftQuickly();
            }
        }
        //printf(" rotating\n");
        break;
    }
    case followingWall:
     //   if (isItATrap()){
       //     myState = lookingForWall;
        //    enteringATrap=1;
         //   startTimeState=getTimeMicroseconds();
        //}
        if (getDistanceFrontWall()<wallDistance && getDistanceRightWall()<maximumWallDistance){
            myState = rotating;
            initialTurningAngle=getAngle();
            desiredAngle = turningAngle;
            //setCarrotPosition(0,turningAngle);
            startTimeState = getTimeMicroseconds();
//            printf("transitioning from following for a wall to rotating\n");

        }
        else if (getDistanceRightWall()>maximumWallDistance){
            if (getDistanceFrontWall()<maximumWallDistance){

                myState = rotating;
                initialTurningAngle=getAngle();
                desiredAngle = -turningAngle;
                //setCarrotPosition(0,-turningAngle);
                startTimeState = getTimeMicroseconds();

            }
            else{
                myState=lookingForWall;
                startTimeState = getTimeMicroseconds();
            }
//            printf("transitioning from following to  looking \n");


        }
        else if (difTime>maxTimeFollowingWall){
            fastWiggling=1;
        }
        else{
            double carrotAngle;
            double realWallDistance = getDistanceRightWall();
            double distanceToMoveToWall = realWallDistance-wallDistance;
            carrotAngle = cartesianCoordinatesToAngle(carrotDistance, -distanceToMoveToWall)*proportionalGain;
            setCarrotPosition(carrotDistance,carrotAngle);
//            printf("Following wall\n");

        }

        break;
    }

}
//Copy of the above. It is bad practice, but I'm feeling lazy at the moment

//Copy of the above. It is bad practice, but I'm feeling lazy at the moment
void states::wallFollowLeft(double carrotDistance,
                            double proportionalGain,
                            double wallDistance,
                            double maximumWallDistance,
                            double turningAngle,
                            double maxLookingForWallTime,
                            double wiggleTime,
                            double fastWiggleTime,
                            double maxTimeFollowingWall){
    enum wallFollowState{lookingForWall, rotating, followingWall};
    static long long int startTimeState;
    // EXPERIMENTAL ADDITION    // if left,front,right shortIRs are low,
    static int enteringATrap=0; // you need to turn around NOW!
    // EXPERIMENTAL ADDITION    // you're entering a trap!
    static int stuckOnACorner=0;
    static int wiggleDirection=0;
    static int wiggling=0;
    static int fastWiggling=0;
    static double initialTurningAngle =0;
    static int desiredAngle=0;
    static wallFollowState myState;
    long long int difTime;
    difTime=(getTimeMicroseconds()-startTimeState)/1000;

    wallFollowed=1;
    if(!wallFollowing){
        myState = lookingForWall;
        startTimeState=getTimeMicroseconds();
        wiggling=0;
        fastWiggling=0;
        stuckOnACorner=0;

        // EXPERIMENTAL ADDITION
        enteringATrap=0;
        // EXPERIMENTAL ADDITION
    }
    if(stuckOnACorner){
        //printf("Stuck on a corner \n");
        if (difTime<wiggleTime/2){
            setCarrotPosition(0,-50);
            return;

        }
        else if (difTime<wiggleTime*3.0/2){
                sharpCurveToTheRightBack();

        }
        else{
            startTimeState = getTimeMicroseconds();
            stuckOnACorner=0;
        }
    }
    else {
//        printf("Not stuck on a corner\n");

        if(getDistanceFrontWall()<WALL_FOLLOW_MINIMUM_DISTANCE_WALL && getDistanceLeftWall()<WALL_FOLLOW_MINIMUM_DISTANCE_WALL){
            if (difTime>WALL_FOLLOW_MINIMUM_TIME_BEFORE_WIGGLE_MS){
                stuckOnACorner=1;
                startTimeState = getTimeMicroseconds();
            }
        }
//        if(enteringATrap){
//            if (difTime>WALL_FOLLOW_TRAP_TIME_MS || finishedTurningNDegreesQuickly){
//                startTimeState=getTimeMicroseconds();
//                enteringATrap=0;
//            }
//            else{
//                turnNDegreesQuickly(180);
//                //mediumCurveToTheLeft();
//            }
//            return;
//        }
        if(wiggling){
           // printf("Wiggling \n");
            if (difTime>wiggleTime){
                startTimeState=getTimeMicroseconds();//previousStartTimeState;
                wiggling=0;
                fastWiggling=0;

//                printf("Not wiggling anymore \n");
            }
            else{
                switch(wiggleDirection){
                case(0):
                    sharpCurveToTheRightBack();
                    break;
                case(1):
                    sharpCurveToTheLeftBack();
                    break;
                case(2):
                    sharpCurveToTheRightBack();
                    break;
                case(3):
                    sharpCurveToTheLeftBack();
                    break;
                }
            }
            return;
        }
        else if(fastWiggling){
            // printf("Wiggling \n");
             if (difTime>fastWiggleTime){
                 startTimeState=getTimeMicroseconds();//previousStartTimeState;
                 fastWiggling=0;
 //                printf("Not wiggling anymore \n");
             }
             else{
                 switch(wiggleDirection){
                 case(0):
                     sharpCurveToTheRightBack();
                     break;
                 case(1):
                     sharpCurveToTheLeftBack();
                     break;
                 case(2):
                     sharpCurveToTheRightBack();
                     break;
                 case(3):
                     sharpCurveToTheLeftBack();
                     break;
                 }
             }
             return;
         }
    }

    switch (myState) {
    case lookingForWall:

        if (getDistanceFrontWall()<wallDistance){
            if (getDistanceLeftWall()<maximumWallDistance){
                myState = rotating;
                initialTurningAngle=getAngle();
                desiredAngle = turningAngle;

                //setCarrotPosition(0,turningAngle);
                startTimeState = getTimeMicroseconds();

            }
            else{

                myState = rotating;
                initialTurningAngle=getAngle();
                desiredAngle = turningAngle;

                //setCarrotPosition(0,turningAngle);
                startTimeState = getTimeMicroseconds();
            }
//            printf("transitioning from looking for a wall to rotating\n");
        }
        else if (getDistanceLeftWall()<wallDistance){
            //printf("transitioning from looking for a wall to following a wall");
            myState=followingWall;
        } else{
            if(difTime>maxLookingForWallTime){
                //printf("starting to wiggle\n");
                //previousStartTimeState = startTimeState;
                startTimeState=getTimeMicroseconds();
                wiggling =true;
                wiggleDirection++;
                wiggleDirection%=4;
                stuckOnACorner=0;
                return;

            }
            else if(difTime>WALL_FOLLOW_TIME_OUT_LOOKING_MS){
                setCarrotPosition(carrotDistance,0);
            }
            else{
                sharpCurveToTheLeft();
                return;
            }
            //printf("Im looking and my distance is %lf\n", getDistanceFrontWall());

        }
        break;

    case rotating:{
        double myAngle = getAngle();
        double angleDif =myAngle-initialTurningAngle;
        double absAngleDif =abs(angleDif);
        if (absAngleDif>abs(desiredAngle)|| abs(absAngleDif-desiredAngle)<10|| difTime>WALL_FOLLOW_TIME_OUT_ROTATING_MS){
            myState=followingWall;
            startTimeState = getTimeMicroseconds();
            stop();
//            printf("transitioning from rotating to following; myangle =%lf, initial angle = %lf, difference=%lf\n", myAngle, initialTurningAngle, angleDif);
        }else{
            if(desiredAngle>0){
                turnToTheRightQuickly();
            }
            else{
                turnToTheLeftQuickly();
            }
        }
        //printf(" rotating\n");
        break;
    }
    case followingWall:
     //   if (isItATrap()){
       //     myState = lookingForWall;
        //    enteringATrap=1;
         //   startTimeState=getTimeMicroseconds();
        //}
        if (getDistanceFrontWall()<wallDistance && getDistanceLeftWall()<maximumWallDistance){
            myState = rotating;
            initialTurningAngle=getAngle();
            desiredAngle = turningAngle;
            //setCarrotPosition(0,turningAngle);
            startTimeState = getTimeMicroseconds();
//            printf("transitioning from following for a wall to rotating\n");

        }
        else if (getDistanceLeftWall()>maximumWallDistance){
            if (getDistanceFrontWall()<maximumWallDistance){

                myState = rotating;
                initialTurningAngle=getAngle();
                desiredAngle = -turningAngle;
                //setCarrotPosition(0,-turningAngle);
                startTimeState = getTimeMicroseconds();

            }
            else{
                myState=lookingForWall;
                startTimeState = getTimeMicroseconds();
            }
//            printf("transitioning from following to  looking \n");


        }
        else if (difTime>maxTimeFollowingWall){
            fastWiggling=1;
        }
        else{
            double carrotAngle;
            double realWallDistance = getDistanceLeftWall();
            double distanceToMoveToWall = realWallDistance-wallDistance;
            carrotAngle = cartesianCoordinatesToAngle(carrotDistance, -distanceToMoveToWall)*proportionalGain;
            setCarrotPosition(carrotDistance,carrotAngle);
//            printf("Following wall\n");

        }

        break;
    }

}


void states::wallFollowLeftFast(){
    //enum statesWallFollowFast;
    //static int itIsATrap;
    //if(!isItATrap()){
        wallFollowLeft(FAST_WALL_FOLLOW_CARROT_DISTANCE,
                       FAST_WALL_FOLLOW_PROPORTIONAL_GAIN,
                       FAST_WALL_FOLLOW_WALL_DISTANCE,
                       FAST_WALL_FOLLOW_MAXIMUM_WALL_DISTANCE_INCHES,
                       FAST_WALL_FOLLOW_TURNING_ANGLE,
                       FAST_WALL_FOLLOW_LOOKING_MAX_TIME);
    //}
}


///////If you pass no parameter, than color is -1, and COLOR will be detected by the color sensor.
/// Should be used with no parameter in competition, in order to always use the color sensor
void states::collectBlock(int color){
    static int counter=0;
    static long long int startTimeState;
    static int myColor;
    static int receivedColor;
    static int detectedBlock=0;
    enum collectBlockState{resettingStart, moving, grabing,lifting, sorting,releasing, swipping, resettingFinish, backingOff};
    static collectBlockState myState=resettingStart;
    collectedBlocks=1;

    if(!collectingBlocks){
        startTimeState = getTimeMicroseconds();
        myState=resettingStart;
        finishedCollectingBlock=0;
        myColor=color;
        receivedColor=(myColor!=-1);
        detectedBlock=0;
        successfullyCollectedBlock=0;
    }

    long long int difTime;
    difTime=(getTimeMicroseconds()-startTimeState)/1000;
    if(detectedCube()){
        detectedBlock=1;
    }
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
            if(detectedBlock){
                myState=grabing;
                myServosControl->hookBlock();
                startTimeState = getTimeMicroseconds();
                successfullyCollectedBlock=1;
            }
            else{
                successfullyCollectedBlock=0;
                myState =resettingFinish;
                startTimeState = getTimeMicroseconds();
                setCarrotPosition(-BLOCK_COLLECT_DISTANCE_MOVE_BACK,0);
                counter++;
            }
        }
        break;
    case(grabing): 
        if(difTime>BLOCK_COLLECT_GRAB_TIME_MS){
            myState=lifting;
            myServosControl->raiseBlock();
            setCarrotPosition(-BLOCK_COLLECT_DISTANCE_MOVE_BACK,0);
            startTimeState = getTimeMicroseconds();
        }
        break;
    case(lifting):
        if(difTime>BLOCK_COLLECT_LIFT_TIME_MS){
            myState=sorting;
            if(!receivedColor){
                myColor=isCubeRed();
            }
            else{
                myColor=color;
            }
            if (myColor == CUBE_GREEN && areWeRed() || myColor == CUBE_RED && !areWeRed()){
                myServosControl->sortLeft();
                myColorCount += 1;
            }

            else{
                myServosControl->sortRight();
                theirColorCount += 1;
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
            if (counter <BLOCK_COLLECT_COUNTER_LIMIT){
                finishedCollectingBlock=1;
                myState=resettingStart;
                collectingBlocks=0;
                collectedBlocks=0;
            }
            else{
                myState=backingOff;
            }
        }
        break;
    case(backingOff):
        if(difTime>BLOCK_COLLECT_BACKINGOFF_TIME_MS){
            finishedCollectingBlock=1;
            myState=resettingStart;
            collectingBlocks=0;
            collectedBlocks=0;
            counter=0;
        }
        else{
            wallFollow();
        }
    }



}

void states::goToPoint(double distance, double angle){
    enum goToPointState{turning, going};
    static long long int startTimeState;
    long long int difTime;
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
        startTimeState=getTimeMicroseconds();
    }
    difTime=(getTimeMicroseconds()-startTimeState)/1000;
    switch(myState){
    case turning:
        angleError=getAngleToCarrot();
        if ((angleError<=GO_TO_POINT_PRECISION_ANGLE&& -angleError<=GO_TO_POINT_PRECISION_ANGLE)||
                difTime>GO_TO_POINT_TURNING_TIMEOUT_MS){
            setCarrotPosition(myDistance,angleError);
            myState=going;
            startTimeState=getTimeMicroseconds();
        }
        break;
    case going:
        if(getDistanceToCarrot()<=GO_TO_POINT_PRECISION_INCHES ||
                startTimeState>GO_TO_POINT_POSITION_TIMEOUT_MS){
            finishedGoingToPoint=1;
            wentToPoint=0;
        }
        break;
    }
}

void states::followPoint(double distance, double angle){
    enum followPointStates {updating,updated, timeout};
    static followPointStates myState = updating;
    static long long int startTimeState;
    static long long int startTimeProcedure;

    followedPoint=1;
    if(!followingPoint){
        finishedFollowingPoint=0;
        startTimeState = getTimeMicroseconds();
        startTimeProcedure =getTimeMicroseconds();
        myState=updating;
    }
    long long int difTimeStartState;
    long long int difTimeStartProcedure;
    difTimeStartProcedure=(getTimeMicroseconds()-startTimeProcedure)/1000;

    if(difTimeStartProcedure>FOLLOW_POINT_TIMEOUT_MS){
        if(myState!=timeout){
            myState=timeout;
            startTimeState=getTimeMicroseconds();
        }
    }
    difTimeStartState=(getTimeMicroseconds()-startTimeState)/1000;

    switch(myState){
    case updating:
        if(!finishedFollowingPoint){
            if (distance<FOLLOW_POINT_DISTANCE_INCHES+FOLLOW_POINT_PRECISION_INCHES){
                finishedFollowingPoint=1;
                followedPoint=0;
                setCarrotPosition(0,0);
            }
            else{
                distance-=FOLLOW_POINT_DISTANCE_INCHES;
                if (distance>FOLLOW_POINT_CARROT_DISTANCE)
                    setCarrotPosition(FOLLOW_POINT_CARROT_DISTANCE,angle);
                else
                    setCarrotPosition(distance,angle);

            }
        }
        startTimeState = getTimeMicroseconds();
        myState=updated;
        break;
    case updated:
        if(difTimeStartState>FOLLOW_POINT_UPDATE_RATE_MS){
            startTimeState = getTimeMicroseconds();
            myState=updating;
        }
        break;
    case timeout:
        if(difTimeStartState<FOLLOW_POINT_TIMEOUT_WALLFOLLOW_MS){
            wallFollow();
        }
        else{
            followedPoint=0;
            //reset
        }
        break;
    }
}

void states::turnNDegreesQuickly(int angle){
    enum turningStates {turning,turned};
    static turningStates myState = turning;
    static long long int startTimeState;
    static double startAngle;
    turnedNDegreesQuickly=true;


    if(!turningNDegreesQuickly){
        finishedTurningNDegreesQuickly=0;
        startTimeState = getTimeMicroseconds();
        myState=turning;
        startAngle=getAngle();
    }
    long long int difTime;
    double difAngle;
    difTime=(getTimeMicroseconds()-startTimeState)/1000;
    difAngle= getAngle()- startAngle;
    switch(myState){
    case turning:
        if(!finishedTurningNDegreesQuickly){
            if (((difAngle>=angle)&&(angle>=0))||((difAngle<=angle)&&(angle<=0))){
                finishedTurningNDegreesQuickly=1;
                turnedNDegreesQuickly=0;
                myState=turned;
                setCarrotPosition(0,0);
            }
            else if(difTime>TURN_N_DEGREES_QUICKLY_TIMEOUT_MS){
                myState=turned;
                turnedNDegreesQuickly=0;
                finishedTurningNDegreesQuickly=1;

            }
            else{
                if(angle>0)
                    turnToTheRightQuickly();
                else
                    turnToTheLeftQuickly();
            }
        }
        break;
    case turned:
        break;
    }
}

void states::turnNDegreesSlowly(int angle){
    enum turningStates {turning,turned};
    static turningStates myState = turning;
    static long long int startTimeState;
    static double startAngle;
    turnedNDegreesSlowly=true;


    if(!turningNDegreesSlowly){
        finishedTurningNDegreesSlowly=0;
        startTimeState = getTimeMicroseconds();
        myState=turning;
        startAngle=getAngle();
    }
    long long int difTime;
    double difAngle;
    difTime=(getTimeMicroseconds()-startTimeState)/1000;
    difAngle= getAngle()- startAngle;
    switch(myState){
    case turning:
        if(!finishedTurningNDegreesSlowly){
            if (((difAngle>=angle)&&(angle>=0))||((difAngle<=angle)&&(angle<=0))){
                finishedTurningNDegreesSlowly=1;
                turnedNDegreesSlowly=0;
                myState=turned;
                setCarrotPosition(0,0);
            }
            else if(difTime>TURN_N_DEGREES_SLOWLY_TIMEOUT_MS){
                myState=turned;
                turnedNDegreesSlowly=0;
                finishedTurningNDegreesSlowly=1;

            }
            else{
                if(angle>0)
                    turnToTheRightSlowly();
                else
                    turnToTheLeftSlowly();
            }
        }
        break;
    case turned:
        break;
    }
}



void states::stop(){
    setCarrotPosition(0,0);
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

void states::sharpCurveToTheRightBack(){
    setCarrotPosition(-SHARP_CURVE_CARROT_DISTANCE,SHARP_CURVE_CARROT_ANGLE);
}

void states::sharpCurveToTheLeftBack(){
    setCarrotPosition(-SHARP_CURVE_CARROT_DISTANCE,-SHARP_CURVE_CARROT_ANGLE);
}

void states::turnToTheRightQuickly(){
    setCarrotPosition(0,TURN_QUICKLY_ANGLE);
}

void states::turnToTheLeftQuickly(){
    setCarrotPosition(0,-TURN_QUICKLY_ANGLE);
}

void states::turnToTheRightSlowly(){
    setCarrotPosition(0,TURN_SLOWLY_ANGLE);
}

void states::turnToTheLeftSlowly(){
    setCarrotPosition(0,-TURN_SLOWLY_ANGLE);
}
//Simple procedures (No states, no timeouts)

int states::detectedCube(){
    return (mySensors->frontUltraShortIRData<ULTRASHORTIR_THRESHOLD);
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


volatile double states::getDistanceLeftWall(){
    return mySensors->leftShortIRData;
}

volatile double states::getDistanceRightWall(){
    return mySensors->rightShortIRData;
}

volatile double states::getDistanceFrontWall(){
    return mySensors->frontShortIRData;
}

volatile int states::isItATrap(){
    return ((getDistanceLeftWall()+getDistanceRightWall())<TRAPPED_SIDE_THRESHOLD)
            && getDistanceFrontWall()<TRAPPED_FRONT_THRESHOLD;
            //getDistanceleftWall() < LEFT_SHORTIR_TRAPPED_THRESHOLD &&
            //getDistanceRightWall() < RIGHT_SHORTIR_TRAPPED_THRESHOLD &&
            //getDistanceFrontWall() < FRONT_SHORTIR_TRAPPED_THRESHOLD);
}

double states::getDistanceToPurpleLine(){
    return 0;//myImageProcessor->getDistToPurpleLine();
}

double states::getInitialAngleToPurpleLine(){
    return 0;
}

int states::foundPurpleLine(){
    return 0;//myImageProcessor->detectedPurpleLine();
}



int states::goingOppositeToPower(){
    int answ = 0;
    answ+=(myMotorControl->normalizedLeftWheelSpeed>=0 && myMotorControl->leftMotorPower<0);
    answ+=(myMotorControl->normalizedLeftWheelSpeed<=0 && myMotorControl->leftMotorPower>0);

    answ+=(myMotorControl->normalizedRightWheelSpeed>=0 && myMotorControl->rightMotorPower<0);
    answ+=(myMotorControl->normalizedRightWheelSpeed<=0 && myMotorControl->rightMotorPower>0);

    return answ;
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

int states::isCubeRed(){
    return mySensors->colorSensorData>0.5;
}

int states::areWeRed(){
    return mySensors->teamData;
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

    if (turnedNDegreesSlowly)
        turningNDegreesSlowly=1;
    else
        turningNDegreesSlowly=0;
}


states::~states(){

}
