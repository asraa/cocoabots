#ifndef STATEGOINGTOPURPLELINE_H
#define STATEGOINGTOPURPLELINE_H
#include "states.h"

class stateGoingToPurpleLine: public states
{
public:
    stateGoingToPurpleLine(states * previousState);
    void processData();

};

#endif // STATEGOINGTOPURPLELINE_H
