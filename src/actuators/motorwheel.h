#ifndef MOTORWHEEL_H
#define MOTORWHEEL_H
#include "mraa.hpp"
class motorWheel
{
public:
    motorWheel(int dirPinN, int pwmInd);
    mraa::Gpio dirPin;
    int pwmIndex;
private:

};

#endif // MOTORWHEEL_H
