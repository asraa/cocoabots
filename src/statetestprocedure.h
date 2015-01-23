#ifndef STATETESTPROCEDURE_H
#define STATETESTPROCEDURE_H
#include "states.h"
class stateTestProcedure: public states
{
public:
    stateTestProcedure(states * previousState);

    //processData does nothing;
    void processData();


};

#endif // STATETESTPROCEDURE_H
