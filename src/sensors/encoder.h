#ifndef ENCODER_H
#define ENCODER_H

#define GEAR_RATIO 30.0
#define EDGES_PER_ROTATION 32.0

#include "mraa.hpp"
#include <thread>
#include <time/sys.h>
#include <unistd.h>

class encoder
{
  public:
    encoder(int dirPin, int encPin);
    ~encoder();
    long long getData();
    double getRotations();
    static void run(void* encoderSensorPointer);
    static void edge_handler(void* encoderSensorPointer);

    std::thread *runThread;
    mraa::Gpio dirGpio;
    mraa::Gpio encGpio;

    int running;
    long long edgeCount;
    double rotations;

    private:

};


#endif
