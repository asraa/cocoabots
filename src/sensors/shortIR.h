#ifndef SHORTIR_H
#define SHORTIR_H
#include "mraa.hpp"

#define SHORTIR_CM 1
#define SHORTIR_INC 0

class shortIR
{
  public:
	mraa:Gpio * data_gpio;    
	shortIR(int dataPin);
    long timing();
    int ranging(int sys);

    private:
    int dataPin;
    long duration, distance_cm, distance_inc;
    
};


#endif
