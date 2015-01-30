#ifndef STATEAPPROACHBLOCK_H
#define STATEAPPROACHBLOCK_H
#include "states.h"
class stateApproachBlock:public states
{
public:
    stateApproachBlock(states * previousState);
    void processData();
    ~stateApproachBlock();
};

#endif // STATEAPPROACHBLOCK_H
