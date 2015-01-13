#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#define MS 1000
#define GYRO_DATA_OKAY_MASK 0x0C000000
#define GYRO_DATA_OKAY 0x04000000

#include "mraa.hpp"
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <sys/time.h>


class gyroscope
{
  public:
    ///initializes the Gyroscope
    gyroscope(int chipSelectPin, int spiPin);
    ~gyroscope();
    float getTotal();
    float getReading();
    float getTime();
    static void run(void* gyroscopeSensorPointer);

    std::thread *runThread;
    mraa::Gpio chipSelectGpio;
    mraa::Spi spiSpi;

    int running;
    uint8_t rxBuf[2];
    uint8_t writeBuf[4];
    unsigned int sensorRead;
    float total;
    struct timeval tv;
    short reading;
    float msf;
    int init;
};


#endif
