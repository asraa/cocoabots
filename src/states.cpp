#include "states.h"

states::states(motorsControl * motorControlPointer,
               servosControl * servoControlPointer,
               sensorsModule * sensorsPointer,
               utils *utilsPointer):
    myMotorControl(motorControlPointer),
    myServosControl(servoControlPointer),
    mySensors(sensorsPointer),
    myUtils(utilsPointer),
    nextState(this)
{
    startTimeStateMicroseconds = mySensors->timeMicrosecondsSinceEpoch;
}

int states::getTimeRemainingGameSeconds(){
    return myUtils->gameTimeRemaining();
}


template <class newState>
states * states::transitionToState(){
    return new newState(myMotorControl,myServosControl, mySensors, myUtils);
}

states * states::getNextState(){
    return nextState;
}

long long int states::getRunningTimeMicroSeconds(){
    return mySensors->timeMicrosecondsSinceEpoch-startTimeStateMicroseconds;
}
