#include "cocoabot.h"
#include <cstring>


//default action, no parameters
cocoabot::cocoabot()
{
    this->setup();
    this->run();
}


void cocoabot::setup()
{
    setupSensors();
    setupUtils(); //time, etc.
}

void cocoabot::setupSensors()
{
    //Nothing to do here at the moment, everything is initialized with the class.
    //Maybe the gyroscope need some special setup.
}

void cocoabot::setupUtils(){
    //Restart Utils for timer
    myutils = utils();
}


void cocoabot::run()
{
    //Should run in 30ms
    while (myutils.gameTimeRemaining() > 0) {
        inputState input = this->makestate(mysensors); //Instead of creating a new inputState everytime, it would be bettter to have only one as a variable and update it.
        processState proc = this->currentState.process(input);
        this->currentState = proc.getNextState();
        this->processOutputs(proc.getOutputs());

    }
}


inputState cocoabot::makestate(struct sensors sensorsStruct)
{

}

void cocoabot::processOutputs(outputs instructions)
{
    // motor movement controller read instructions and update voltages on the motors
    // lift movement controller read instructions and update voltages on the motors

}


///Tests after this point.

//non default action; useful for tests, etc
cocoabot::cocoabot(int argc, char **argv)
{
    if(strcmp(argv[1],"wallFollow")==0){
        wallFollowTest();
        //run Custom test

    }
    else if (strcmp(argv[1],"rotate")==0){
        rotateTest();
    }

    else if (strcmp(argv[1],"moveToWall")==0){
        moveToWallTest();
    }
}


void cocoabot::wallFollowTest()
{
    //int trigerPin=1;
    //int echoPin=2;
    //mysensors.fUS= ultrasonic(trigerPin, echoPin);
    //do things

}

void cocoabot::rotateTest()
{

}

void cocoabot::moveToWallTest()
{
    //int trigerPin=1;
    //int echoPin=2;
    //mysensors.fUS= ultrasonic(trigerPin, echoPin);
    //do things

}
