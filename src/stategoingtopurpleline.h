#ifndef STATEGOINGTOPURPLELINE_H
#define STATEGOINGTOPURPLELINE_H
#include "states.h"

class stateGoingToPurpleLine: public states
{
public:
    stateGoingToPurpleLine(states * previousState, int areWeInside);
    void processData();
    ~stateGoingToPurpleLine();
};

#endif // STATEGOINGTOPURPLELINE_H
