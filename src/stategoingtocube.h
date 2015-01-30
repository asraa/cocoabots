#ifndef STATEGOINGTOCUBE_H
#define STATEGOINGTOCUBE_H
#include "states.h"
class stateGoingToCube:public states
{
public:
    stateGoingToCube(states * previousState, long long int waitTimeMS=GO_TO_CUBE_WAIT_TIME_MS);
    void processData();
    ~stateGoingToCube();
    long long int mywaitTimeMS;
};

#endif // STATEGOINGTOCUBE_H
