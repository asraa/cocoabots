#ifndef BUTTON_H
#define BUTTON_H
#include "mraa.hpp"
#include "sensorssuperclass.h"
class button: public sensorsSuperClass
{
public:
    mraa::Gpio * data_gpio;
    button(int Pin);
    int readData();
    double getData();
private:
    int myDataPin;
};
#endif // BUTTON_H
