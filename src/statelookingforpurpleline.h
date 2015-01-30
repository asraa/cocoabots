#ifndef STATELOOKINGFORPURPLELINE_H
#define STATELOOKINGFORPURPLELINE_H
#include "states.h"
#include "stategoingtocube.h"
class stateLookingForPurpleLine: public states
{
public:
    stateLookingForPurpleLine(states* previousState);
    void processData();

    long long int startTimeState;


};

#endif // STATELOOKINGFORBLOCKS_H
