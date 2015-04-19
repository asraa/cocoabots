#ifndef STATEALLIGNINGPURPLELINE_H
#define STATEALLIGNINGPURPLELINE_H
#include "states.h"
class stateAlligningPurpleLine: public states
{
public:
    stateAlligningPurpleLine(states * previousState, int isHomeBaseOnOurBack);
    void processData();
    ~stateAlligningPurpleLine();
};

#endif // STATEALLIGNINGPURPLELINE_H
