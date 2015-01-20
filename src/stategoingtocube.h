#ifndef STATEGOINGTOCUBE_H
#define STATEGOINGTOCUBE_H
#include "states.h"
class stateGoingToCube:public states
{
public:
    stateGoingToCube(states * previousState);
    void processData();
};

#endif // STATEGOINGTOCUBE_H
