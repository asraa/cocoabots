#ifndef STATESCREEPYBASE_H
#define STATESCREEPYBASE_H
#include "../states.h"
#include "instructions.h"

class statesCreepyBase :public states
{
public:
    statesCreepyBase(motorsControl * motorControlPointer,
                      servosControl * servoControlPointer,
                      sensorsModule * sensorsPointer,
                      utils * utilsPointer,
                      instructions *instructionsPointer);
    statesCreepyBase(statesCreepyBase * previousCreepyState);
    virtual ~statesCreepyBase();
protected:
    instructions *myInstructions;
};

#endif // STATESCREEPYBASE_H
