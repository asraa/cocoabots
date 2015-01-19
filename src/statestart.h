#ifndef STATESTART_H
#define STATESTART_H
#include "states.h"

class stateStart : public states
{
public:
    stateStart(motorsControl * motorControlPointer,
               servosControl * servoControlPointer,
               sensorsModule * sensorsPointer,
               utils * utilsPointer);
    stateStart(states * previouState);
     void processData();
};

#endif // STARTSTATE_H
