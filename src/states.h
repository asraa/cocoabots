#ifndef STATES_H
#define STATES_H
#include "actuators/motorscontrol.h"
#include "actuators/servoscontrol.h"
#include "sensorsmodule.h"
#include "imageProcessing/ImageProcessor.h"
#include "configFile.h"
#include <string>
#include "utils.h"
class states
{
public:
    states(motorsControl * motorControlPointer,
           servosControl * servoControlPointer,
           sensorsModule * sensorsPointer,
           ImageProcessor * imageProcessorPointer,
           utils * utilsPointer);
    states(states * previouStatePointer);
    virtual ~states();
    states * getNextState(); //Can return this, or a new state
    virtual void processData() = 0; //This is the brain of the robot
    std::string name;
    std::string getName();

    //Here are the helper functions for the states
    int getTimeRemainingGameSeconds();
    long long int getRunningTimeMicroSeconds();
    long long int getTimeMicroseconds();
    double getAngle();

    //Positive is defined to be to the right, i.e. a negative side distance will make it turn to the left
    double cartesianCoordinatesToAngle(double frontDistance, double sideDistance);
    volatile double getDistanceLeftWall();
    volatile double getDistanceRightWall();
    volatile double getDistanceFrontWall();
    volatile int isItATrap();
    double getAngleDifference(double angle1, double angle2);
    void setCarrotPosition(double distance, double angle);
    double getDistanceToCarrot();
    double getAngleToCarrot();
    int foundCube();
    int detectedCube();
    double getDistanceNearestCube();
    double getAngleNearestCube();
    int getColorNearestCube();
    double abs(double number);
    int isCubeRed();

    int areWeRed(); //TODO WRITE THIS <<<<<<<<<<<<<<<<<<<<<< TODO

    int goingOppositeToPower();

    //Here are the procedures that can be used in all states.
    //Many of them are implmented as state machines, so they should have two variables associated
    //So that they could be cleaned. Take as an example the wall follow procedure and startProcessData
    // and finish Process Data.

    void sharpCurveToTheRight();
    void sharpCurveToTheLeft();
    void sharpCurveToTheRightBack();
    void sharpCurveToTheLeftBack();
    void mediumCurveToTheRight();
    void mediumCurveToTheLeft();
    void curveToTheRight();
    void curveToTheLeft();
    void turnToTheRightSlowly();
    void turnToTheLeftSlowly();
    void turnToTheRightQuickly();
    void turnToTheLeftQuickly();
    void stop();

    double getDistanceToPurpleLine();
    double getInitialAngleToPurpleLine();

    //wallFollow(); Follow a wall on the right
    void wallFollowLeftFast();
    void wallFollowLeft(double carrotDistance=WALL_FOLLOW_CARROT_DISTANCE_INCHES,
                        double proportionalGain=1,
                        double wallDistance=WALL_FOLLOW_WALL_DISTANCE_INCHES,
                        double maximumWallDistance =WALL_FOLLOW_MAXIMUM_WALL_DISTANCE_INCHES,
                        double turningAngle = 45,
                        double maxLookingForWallTime = WALL_FOLLOW_LOOKING_MAX_TIME,
                        double wiggleTime = WALL_FOLLOW_WIGGLE_TIME_MS,
                        double fastWiggleTime = WALL_FOLLOW_FAST_WIGGLE_TIME_MS,
                        double maxTimeFollowingWall = WALL_FOLLOW_FOLLOWING_MAX_TIME); //procedure to wall follow
    void wallFollowRight(); //procedure to wall follow
    void wallFollow(); //procedure to wall follow
    int wallFollowed;  //Variable that says if we have wall followed on the current iteration of the state machine process data
    int wallFollowing; //Variable that says if we have wall folowed on the previous iteration of the state machine process data

    void collectBlock(int color=-1);
    int collectedBlocks;
    int collectingBlocks;
    int finishedCollectingBlock; //I assume that we always finish collecting it.

    void goToPoint(double distance, double angle);
    int wentToPoint;
    int goingToPoint;
    int failedGoingToPoint;
    int finishedGoingToPoint;

    void followPoint(double distance, double angle);
    int followedPoint;
    int followingPoint;
    int failedFollowingPoint;
    int finishedFollowingPoint;

    void turnNDegreesQuickly(int angle);
    int turnedNDegreesQuickly;
    int turningNDegreesQuickly;
    int finishedTurningNDegreesQuickly;

    void turnNDegreesSlowly(int angle);
    int turnedNDegreesSlowly;
    int turningNDegreesSlowly;
    int finishedTurningNDegreesSlowly;

    void startProcessingProceduresManual();
    void finishProcessingProceduresManual();
protected:
    //Functions that should always be called.
    void startProcessData(); //Should be called in the beginning of process data.
                            //Takes care of procedures that have state machines inside them.
    void finishProcessData(); // Should be called in the end of process data.
                            //Takes care of procedures that have state machines inside them.


    states * nextState;
    //Here are defined all the data that the states have acess to.
    long long startTimeStateMicroseconds;
    sensorsModule * mySensors;
    motorsControl * myMotorControl;
    servosControl * myServosControl;
    ImageProcessor * myImageProcessor;
    utils * myUtils;

};



#endif // STATES_H
