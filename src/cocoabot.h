/*Our main file, that starts the robot, defines our integration tests and contains the main loop.
 */

#ifndef COCOABOT_H
#define COCOABOT_H

#include "states.h"
#include "utils.h"
//#include "sensorsdef.h"



class cocoabot
{
public:
    //struct sensors mysensors;
    cocoabot();
    cocoabot(int argc, char **argv);
    state currentState;
    void run();

private:
    void setup();
    void setupSensors();
    void setupUtils();
    void processOutputs(outputs instructions);
    inputState makestate(struct sensors sensorsStruct);
    utils myutils;

///Tests Section:
    void wallFollowTest();
    void rotateTest();
    void moveToWallTest();

};



#endif // COCOABOT_H
