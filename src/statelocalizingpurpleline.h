#ifndef STATELOCALIZINGPURPLELINE_H
#define STATELOCALIZINGPURPLELINE_H
#include "states.h"
class stateLocalizingPurpleLine:public states
{
public:
    stateLocalizingPurpleLine(states * previousState);
    void processData();
    ~stateLocalizingPurpleLine();
};

#endif // STATELOCALIZINGPURPLELINE_H
