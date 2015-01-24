#ifndef STATEAPPROACHBLOCK_H
#define STATEAPPROACHBLOCK_H
#include "states.h"
class stateApproachBlock:public states
{
public:
    stateApproachBlock(states * previousState);
    void processData();
};

#endif // STATEAPPROACHBLOCK_H
