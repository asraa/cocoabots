#ifndef STATECOLLECTINGCUBE_H
#define STATECOLLECTINGCUBE_H
#include "states.h"

class stateCollectingCube: public states
{
public:
    stateCollectingCube(states * previousState, int color);
    void processData();
    int myColor;
};
#endif//STATECOLLECTINGCUBE_H
