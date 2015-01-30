#ifndef STATELOOKINGFORBLOCKS_H
#define STATELOOKINGFORBLOCKS_H
#include "states.h"
#include "stategoingtocube.h"
class stateLookingForBlocks: public states
{
public:
    stateLookingForBlocks(states* previousState);
    void processData();
    long long int startTimeState;
    int cubeFound;
    enum lookingForBlocksState{wallFollowing,looking, comingBack};
    lookingForBlocksState myPrivateState;
    ~stateLookingForBlocks();

};

#endif // STATELOOKINGFORBLOCKS_H
