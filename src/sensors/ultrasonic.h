/*
  Ultrasonic.h - Library for HR-SC04 Ultrasonic Ranging Module.
  Created by ITead studio. Alex, Apr 20, 2010.
  iteadstudio.com
*/


#ifndef ULTRASONIC_H
#define ULTRASONIC_H



#define ULTRASONIC_CM 1
#define ULTRASONIC_INC 0

class ultrasonic
{
  public:
    ultrasonic(int TP, int EP);
    long timing();
    long ranging(int sys);

    private:
    int Trig_pin;
    int Echo_pin;
    long  duration,distacne_cm,distance_inc;
    
};

#endif
