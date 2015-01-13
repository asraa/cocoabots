#ifndef SHORTIR_H
#define SHORTIR_H
#include "mraa.hpp"

#define SHORTIR_CM 1
#define SHORTIR_INC 0

class shortIR
{
  public:

	mraa::Aio * data_aio;    
	shortIR(int dataPin);
    float timing();
    float ranging();

    private:
    int myDataPin;
    long duration, distance_cm, distance_inc;
    
};


#endif
