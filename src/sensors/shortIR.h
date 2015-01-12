#ifndef SHORTIR_H
#define SHORTIR_H
#include "mraa.hpp"

<<<<<<< HEAD

#define ULTRASONIC_CM 1
#define ULTRASONIC_INC 0
=======
#define SHORTIR_CM 1
#define SHORTIR_INC 0
>>>>>>> 93c4d89de8b710cf3e8f835c28f757003be6b73c

class shortIR
{
  public:
    mraa::Gpio * data_gpio;
    shortIR(int dataPin);

    long timing();
    long ranging(int sys);

    private:
    int myDataPin;
    long duration, distance_cm, distance_inc;
    
};


#endif
