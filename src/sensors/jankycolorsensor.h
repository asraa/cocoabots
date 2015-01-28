#ifndef JANKYCOLORSENSOR_H
#define JANKYCOLORSENSOR_H


#include "mraa.hpp"
#include "sensorssuperclass.h"
#include "../configFile.h"

class jankyColorSensor: public sensorsSuperClass
{
  public:
    ///initializes the jankycolorsensor
    mraa::Aio * colorSensorAio;
    jankyColorSensor(int colorSensorPin);
    int runningTotal();
    double rawData();
    double getData();

private:
    int count;
    int myDataPin;

};


#endif
