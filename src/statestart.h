#ifndef STATESTART_H
#define STATESTART_H
#include "states.h"

class stateStart : public states
{
public:
    stateStart(motorsControl * motorControlPointer,
               servosControl * servoControlPointer,
               sensorsModule * sensorsPointer,
               ImageProcessor *imageProcessorPointer,
               utils * utilsPointer);
    stateStart(states * previouState);
     void processData();

     static constexpr char* name= "State Start";//Deprecated, but it is okay
};

#endif // STARTSTATE_H
