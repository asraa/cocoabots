/* Code related to the states of the robot, how to represent inputs and outputs (those two might be moved to another file if convenient later).
 */
#ifndef STATES_H
#define STATES_H
#include "sensorsdef.h"

//Contains data from the sensors, position of the robot, data from the image processing; all datas. It will NOT ask each one of the things for its data, it is just a STORAGE.
//Is populated by the robot, and contains the data in a nice way, e.g.: "touching wall", etc.
//The following is just an example, please change.

class processState;
class inputState;

enum stateType{
    onStart, onSearchForCubes, onDriveToCube, onCollectCube, onDeployCube
};

//Abstract class for the states
class state
{
public:
    state(enum stateType=onStart);
    processState process(inputState input);
    enum stateType myState;
};


class inputState
{
public:
    inputState();
};

//Contains the instructions for the actuators, that will be processed by the robot. It is just a STORAGE.
//The following is just an example, please change.
class outputs
{
public:
    outputs();
};

//Contains the next state for the robot and the outputs (instructions for the actuators). It is just a STORAGE.
class processState
{
public:
    processState();
    state getNextState();
    outputs getOutputs();
    state mynextState;
    outputs myoutputs;
};


#endif // STATE_H
