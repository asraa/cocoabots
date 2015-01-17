// Compile with:
// g++ test_servo_shield.cpp -o test_servo_shield -lmraa
// Controls a servo on port 0 of the servo shield.
#include "mraa.hpp"
#include <cassert>
#include <csignal>
#include <iostream>
#include <cstdint>
#include "color_detector.h"

int running = 1;

void color_detector::write8 (uint8_t reg, uint32_t value)
{
    i2cPin.address(COLOR_I2C_ADDR);
    i2cPin.writeByte(COMMAND_BIT | reg);
    i2cPin.writeByte(value & 0xFF);
}

uint8_t color_detector::read8(uint8_t reg)
{
    i2cPin.address(COLOR_I2C_ADDR);
    i2cPin.writeByte(COMMAND_BIT | reg);
    return i2cPin.readByte();
}

uint16_t color_detector::read16(uint8_t reg)
{
    i2cPin.address(COLOR_I2C_ADDR);
    i2cPin.writeByte(COMMAND_BIT | reg);
    return i2cPin.readWordReg(COLOR_I2C_ADDR);
}

void color_detector::enable(mraa::I2c * i2c){
    //enables power on and RGBC color function
    i2c->address(COLOR_I2C_ADDR);
    i2c->writeReg(ENABLE,ENABLE_PON);
    usleep(3000); //delay 3 msec
    i2c->writeReg(ENABLE, ENABLE_PON | ENABLE_AEN);
}

void color_detector::disable(mraa::I2c *i2c){
    i2c->address(COLOR_I2C_ADDR);
    //disable: write 0's in enable register
    uint8_t reg = i2c->readReg(ENABLE);
    i2c->writeReg(ENABLE, reg & ~(ENABLE_PON | ENABLE_AEN));
}

color_detector::color_detector():i2cPin(6){
    //set integration time and gain
    write8(ATIME, INTEGRATIONTIME_154MS);
    write8(CONTROL, GAIN_4x);
    enable(& i2cPin);
}

uint8_t color_detector::getRedData() {
    usleep(154000);
    return read16(RDATAL);
}

uint8_t color_detector::getGreenData() {
    usleep(154000);
    return read16(GDATAL);
}

uint8_t color_detector::getBlueData() {
    usleep(154000);
    return read16(BDATAL);
}

int main(){
    color_detector ColorDetector;
    std::cout << std::hex << (int)ColorDetector.read8(ID) << std:endl;
    while (running){
        std::cout << std::hex << (int)ColorDetector.getRedData() << std::endl;
        sleep(2.0);
    }
}
