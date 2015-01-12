#ifndef SHORTIR_H
#define SHORTIR_H


#ifndef ULTRASONIC_CM 1
#define ULTRASONIC_INC 0

class shortIR
{
  public:
    shortIR(int dataPin);
    long timing();
    int ranging(int sys);

    private:
    int dataPin;
    long duration, distance_cm, distance_inc;
    
};


#endif
