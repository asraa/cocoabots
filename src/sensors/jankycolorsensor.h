#ifndef JANKYCOLORSENSOR_H
#define JANKYCOLORSENSOR_H


#include "mraa.hpp"
#include "sensorssuperclass.h"

class jankyColorSensor: public sensorsSuperClass
{
  public:
    ///initializes the jankycolorsensor
    mraa::Aio * colorSensorAio;
    jankyColorSensor(int colorSensorPin);
    double getData();

private:
    int myDataPin;

};


#endif
