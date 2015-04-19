#include "statescreepybase.h"

statesCreepyBase::statesCreepyBase(motorsControl *motorControlPointer,
                                   servosControl *servoControlPointer,
                                   sensorsModule *sensorsPointer,
                                   utils *utilsPointer,
                                   instructions *instructionsPointer):states(motorControlPointer,
                                                                             servoControlPointer,
                                                                             sensorsPointer,
                                                                             utilsPointer),
    myInstructions(instructionsPointer)
{
}

statesCreepyBase::statesCreepyBase(statesCreepyBase *previousCreepyState):statesCreepyBase(previousCreepyState->myMotorControl,
                                                                                           previousCreepyState->myServosControl,
                                                                                           previousCreepyState->mySensors,
                                                                                           previousCreepyState->myUtils,
                                                                                           previousCreepyState->myInstructions)
{
}
statesCreepyBase::~statesCreepyBase(){

}
