#include "states.h"


inputState::inputState()
{
}




outputs::outputs()
{
};


processState::processState()
{

}

state processState::getNextState()
{
    return mynextState;

}

outputs processState::getOutputs()
{
    return myoutputs;
}




//Start of the states

state::state(enum stateType myStateType){
    myState = myStateType;
}

processState state::process(inputState input){
    processState result;
    switch (myState){

        case onStart:

            break;

        case onSearchForCubes:

            break;

        case onDriveToCube:

            break;

        case onCollectCube:

            break;

        case onDeployCube:

            break;
    }
}


