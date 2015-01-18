#ifndef STARTSTATE_H
#define STARTSTATE_H
#include "states.h"

class startState : public states
{
public:
    startState(motorsControl * motorControlPointer,
               servosControl * servoControlPointer,
               sensorsModule * sensorsPointer,
               utils * utilsPointer);
     void processData();
};

#endif // STARTSTATE_H
