#ifndef STATECOLLECTINGCUBE_H
#define STATECOLLECTINGCUBE_H
#include "states.h"

class stateCollectingCube: public states
{
public:
    stateCollectingCube(states * previousState);
    void processData();
};
#endif//STATECOLLECTINGCUBE_H
