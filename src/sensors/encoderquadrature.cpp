#include "encoderquadrature.h"

encoderQuadrature::encoderQuadrature(int encA, int encB, int isLeft):
    encAGpio(encA),
    encBGpio(encB)

{
    if(isLeft){
        reversed = ENCODER_OPPOSITE_MOTOR;
    }
    else if (MOTORS_OPPOSITE){
        reversed = ENCODER_OPPOSITE_MOTOR*-1;
    }
    else{
        reversed = ENCODER_OPPOSITE_MOTOR;
    }
    encAGpio.dir(mraa::DIR_IN);
    encAGpio.isr(mraa::EDGE_BOTH, aHandler,(void *) this);

    encBGpio.dir(mraa::DIR_IN);
    encBGpio.isr(mraa::EDGE_BOTH, bHandler, (void *) this);

}


long long encoderQuadrature::getCounts(){
    return count*reversed;
}

double encoderQuadrature::getRotations(){
    double counts = getCounts();
    return ((counts/ (EDGES_PER_ROTATION*2)) / GEAR_RATIO);
}

double encoderQuadrature::getData(){
    return CIRCUMFERENCE_WHEEL*getRotations();
}

int encoderQuadrature::getPhase(int a, int b) {
  if (a == 0 && b == 0) {
    return 0;
  }
  else if (a == 1 && b == 0) {
    return 1;
  }
  else if (a == 1 && b == 1) {
    return 2;
  }
  else if (a == 0 && b == 1) {
    return 3;
  }
  // those are all the possibilities.
}

void encoderQuadrature::updateTick(int prevPhase, int curPhase) {
  // Tick forward (possibly wrapping)
  if (curPhase - prevPhase == 1 ||
      curPhase - prevPhase == -3) {
    count++;
  }
  // Tick backward (possibly wrapping)
  else if (curPhase - prevPhase == -1 ||
           curPhase - prevPhase == 3) {
    count--;
  }
  else {
        //REALLY SHOULDN'T HAPPE FOR OUR SPEEDS.
  }
}

void encoderQuadrature::aHandler(void * myEnc) {
  // Get the gpio handle from the args
  encoderQuadrature * encoder = (encoderQuadrature *)myEnc;
  mraa::Gpio *encA = &(encoder->encAGpio);
  int a = encoder->aState;
  int b = encoder->bState;
  int newA = encA->read();
  encoder->aState = newA;
  int prevPhase = encoder->getPhase(a, b);
  int curPhase = encoder->getPhase(newA, b);
  encoder->updateTick(prevPhase, curPhase);
}

void encoderQuadrature::bHandler(void * myEnc) {
  // Get the gpio handle from the args
  encoderQuadrature * encoder = (encoderQuadrature *)myEnc;
  mraa::Gpio *encB = &(encoder->encBGpio);
  int a = encoder->aState;
  int b = encoder->bState;
  int newB = encB->read();
  encoder->bState = newB;
  int prevPhase = encoder->getPhase(a, b);
  int curPhase = encoder->getPhase(a, newB);
  encoder->updateTick(prevPhase, curPhase);
}

