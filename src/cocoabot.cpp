#include "cocoabot.h"
#include <cstring>


//default action, no parameters
cocoabot::cocoabot()
{
    this->setup();
    this->run();
}


//non default action; useful for tests, etc
cocoabot::cocoabot(int argc, char **argv)
{
    if(strcmp(argv[1],"testUltraSensor")==0){
        //override sensorDefinition:
        int trigerPin=1;
        int echoPin=2;
        mysensors.fUS= ultrasonic(trigerPin, echoPin);
        //run Custom test

    }
    else if (strcmp(argv[1],"testMotors")==0){

    }

}

void cocoabot::setup()
{
    setupSensors();
    setupUtils(); //time, etc.
}

void cocoabot::setupSensors(){
    //Nothing to do here at the moment, everything is initialized with the class.
    //Maybe the gyroscope need some special setup.
}

void cocoabot::setupUtils(){
    //Restart Utils for timer
    myutils = utils();
}


void cocoabot::run(){
    while (myutils.gameTimeRemaining() > 0) {
        inputState input = this->makestate(mysensors);
        processState proc = this->currentState.process(input);
        this->currentState = proc.getNextState();
        this->processOutputs(proc.getOutputs());

    }
}
