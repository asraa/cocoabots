#include "statecreepy.h"

stateCreepy::stateCreepy(motorsControl * motorControlPointer,
                       servosControl * servoControlPointer,
                       sensorsModule * sensorsPointer,
                       utils * utilsPointer,
                       instructions * instructionsPointer):statesCreepyBase(motorControlPointer,
                                                    servoControlPointer,
                                                    sensorsPointer,
                                                    utilsPointer,
                                                    instructionsPointer)
{
    name= "State Creepy";
    myState=followingInstructions;
}

stateCreepy::stateCreepy(statesCreepyBase *previouState):statesCreepyBase(previouState){
    name= "State Creepy";
}

void stateCreepy::processData(){
    startProcessData();
    finishProcessData();
}

void stateCreepy::processData(instructions *myInstructions){
    startProcessData();

    //do stuff
    followInstructions(myInstructions);

    //nextState = new stateWallFollow(this);
    //nextState = new stateLookingForBlocks(this);
    //finish doing stuff
    finishProcessData();
}


void stateCreepy::followInstructions(instructions *myInstructions){
    if(myInstructions->updated){
        setCarrotPosition(myInstructions->distance,myInstructions->angle);

        if(myInstructions->grab){
            myServosControl->hookBlock();
        }
        else{
            myServosControl->unHookBlock();
        }

        if(myInstructions->raiseArm){
            myServosControl->raiseBlock();
        }
        else{
            myServosControl->unRaiseBlock();
        }
        if(myInstructions->sweep){
            myServosControl->sweep();
        }
        else{
            myServosControl->stopSweep();
        }
        myInstructions->updated=false;


    }
}

stateCreepy::~stateCreepy(){

}

