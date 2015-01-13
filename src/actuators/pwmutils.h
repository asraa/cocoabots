//This class is based on the staff example code

#ifndef PWMUTILS_H
#define PWMUTILS_H

#define MS 1000
#define SHIELD_I2C_ADDR 0x40

#include <stdint.h>
#include "mraa.hpp"

class pwmUtils
{
public:
    pwmUtils();
    static const uint8_t registers[16];
    void initPWM(mraa::I2c * i2c);
    void writePWM(int index, double duty);
    void setServoPosition(int index, double duty);
    mraa::I2c i2cPin;

};

#endif // PWMUTILS_H
