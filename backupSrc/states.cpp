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
        case onMoveToWallTest:
            result.myoutputs.robotPosition.currentPosition = 0; //position from input
            result.myoutputs.robotPosition.desiredPosition =0 ; //wall position from input
            result.myoutputs.robotPosition.currentVelocity = 0; //velocity from input
            result.mynextState = *this;
            break;

    }
    return result;
}


