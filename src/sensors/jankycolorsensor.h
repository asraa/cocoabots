#ifndef JANKYCOLORSENSOR_H
#define JANKYCOLORSENSOR_H


#include "mraa.hpp"
#include "sensorssuperclass.h"
#include "../configFile.h"

class jankyColorSensor: public sensorsSuperClass
{
  public:
    ///initializes the jankycolorsensor
    mraa::Gpio * data_gpio;
    jankyColorSensor(int colorSensorPin);
    double getData();

private:
    int myDataPin;

};


#endif
