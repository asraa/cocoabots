#ifndef ENCODER_H
#define ENCODER_H

#define GEAR_RATIO 30.0
#define EDGES_PER_ROTATION 32.0

#include "mraa.hpp"
#include <sys/time.h>
#include <thread>
#include <unistd.h>
#include"sensorssuperclass.h"

class encoder: public sensorsSuperClass
{
  public:
    encoder(int dirPin, int encPin);
    ~encoder();
    long long getCounts();
    double getData();
    double getRotations();
    static void run(void* encoderSensorPointer);
    static void edge_handler(void* encoderSensorPointer);

    //std::thread *runThread;
    mraa::Gpio dirGpio;
    mraa::Gpio encGpio;

    int running;
    long long edgeCount;
    double rotations;

    private:

};


#endif
