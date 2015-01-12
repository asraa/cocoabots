#ifndef SHORTIR_H
#define SHORTIR_H


#define SHORTIR_CM 1
#define SHORTIR_INC 0

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
