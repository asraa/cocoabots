#ifndef jankycolorsensor_H
#define jankycolorsensor_H

#define MS 1000
#define INT_TO_VOLTS 0.0049

#include "mraa.hpp"
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <sys/time.h>
#include <thread>

class jankycolorsensor
{
  public:
    ///initializes the jankycolorsensor
    jankycolorsensor(int colorSensorPin);
    ~jankycolorsensor();
    static void run(void* jankyColorSensorPointer);
    int getData();
    float getDataVolts();

    std::thread *runThread;
    mraa::Aio colorSensorAio;

    int running;
    int dataInt;
    float dataVolt;
};


#endif
