#ifndef COLOR_DETECTOR_H
#define COLOR_DETECTOR_H
#include "mraa.hpp"
#include <stdlib.h>
#include <cassert>
#include <csignal>
#include <iostream>
#define COLOR_I2C_ADDR 0x29

#define INTEGRATIONTIME_154MS 0xC0
#define GAIN_4x 0x01

#define ENABLE 0x00
#define ENABLE_AIEN 0x10 //rgbc interrupt
#define ENABLE_WEN 0x08 //wait time
#define ENABLE_AEN 0x02 //rgbc enable
#define ENABLE_PON 0x01 //power
#define ATIME 0x01 //integration time
#define CONTROL 0x0F //gain
#define COMMAND_BIT 0x80 //command bit

#define CDATAL 0x14 /* Clear channel data */
#define RDATAL 0x16 /* Red channel data */
#define GDATAL 0x18 /* Green channel data */
#define BDATAL 0x1A /* Blue channel data */


class color_detector
{
public:
    color_detector();
    void enable(mraa::I2c * i2c);
    void disable(mraa::I2c * i2c);
    void write8(uint8_t reg, uint32_t value);
    uint8_t read8(uint8_t reg);
    uint16_t read16(uint8_t reg);
    uint8_t getRedData();
    uint8_t getGreenData();
    uint8_t getBlueData();
    mraa::I2c i2cPin;
};


#endif // COLOR_DETECTOR_H
