#include "startstate.h"

startState::startState(motorsControl * motorControlPointer,
                       servosControl * servoControlPointer,
                       sensorsModule * sensorsPointer,
                       utils * utilsPointer):states(motorControlPointer,
                                                    servoControlPointer,
                                                    sensorsPointer,
                                                    utilsPointer)
{
}

void startState::processData(){

}
