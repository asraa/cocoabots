#ifndef STATECREEPY_H
#define STATECREEPY_H
#include "statescreepybase.h"
#include "instructions.h"

class stateCreepy : public statesCreepyBase
{
public:
    stateCreepy(motorsControl * motorControlPointer,
               servosControl * servoControlPointer,
               sensorsModule * sensorsPointer,
               utils * utilsPointer,
                instructions * instructionsPointer);
    stateCreepy(statesCreepyBase * previouState);
     void processData();
     void processData(instructions *myInstructions);
     ~stateCreepy();

     enum myStates{
        followingInstructions,
         recovering
     };
     myStates myState;
     void followInstructions(instructions *myInstructions);

};

#endif // STATECREEPYSTART_H
