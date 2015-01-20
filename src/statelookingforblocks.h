#ifndef STATELOOKINGFORBLOCKS_H
#define STATELOOKINGFORBLOCKS_H
#include "states.h"
#include "stategoingtocube.h"
class stateLookingForBlocks: public states
{
public:
    stateLookingForBlocks(states* previousState);
    void processData();
    int cubeFound;
};

#endif // STATELOOKINGFORBLOCKS_H
