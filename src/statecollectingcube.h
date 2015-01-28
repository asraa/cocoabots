#ifndef STATECOLLECTINGCUBE_H
#define STATECOLLECTINGCUBE_H
#include "states.h"

class stateCollectingCube: public states
{
public:
    stateCollectingCube(states * previousState, int color=-1);
    void processData();
    int myColor;
};
#endif//STATECOLLECTINGCUBE_H
