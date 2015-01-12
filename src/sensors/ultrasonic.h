#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define ULTRASONIC_CM 1
#define ULTRASONIC_INC 0

#include <sys/time.h>
#include "mraa.hpp"

class ultrasonic
{
  public:
    ultrasonic(int TP, int EP);
    void timing();
    long ranging(int sys);

  private:
    int Trig_pin, Echo_pin;
    long duration,distance_cm,distance_in;
};

#endif