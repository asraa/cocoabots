#ifndef STATEWALLFOLLOW_H
#define STATEWALLFOLLOW_H
#include "states.h"
class stateWallFollow : public states
{
public:
    stateWallFollow(states* previousState);
    void processData();
};

#endif // STATEWALLFOLLOW_H
