#include "pwmutils.h"
#include <cassert>

pwmUtils::pwmUtils():i2cPin(6) //Initializes the
{
    initPWM(& i2cPin);
}

//Define the pwm pins (the three things that go outside
const uint8_t pwmUtils::registers[16]= {
    6,   // output 0
    10,  // output 1
    14,  // output 2
    18,  // output 3
    22,  // output 4
    26,  // output 5
    30,  // output 6
    34,  // output 7
    38,  // output 8
    42,  // output 9
    46,  // output 10
    50,  // output 11
    54,  // output 12
    58,  // output 13
    62,  // output 14
    66   // output 15
};

void pwmUtils::initPWM(mraa::I2c * i2c){
    uint8_t writeBuf[2] = {0};
    writeBuf[0] = 0x00; // Write to MODE 1 Register;
    writeBuf[1] = 1 << 4; // Enable Sleep Mode

    i2c->address(SHIELD_I2C_ADDR);
    i2c->write(writeBuf, 2);

    usleep(10 * MS);

    writeBuf[0] = 0xFE; // Write Prescaler Register
    writeBuf[1] = 0xA3; // Set pwm frequency to ~40 Hz

    i2c->address(SHIELD_I2C_ADDR);
    i2c->write(writeBuf, 2);

    writeBuf[0] = 0; // Write to the MODE 1 register
    writeBuf[1] = 1 << 5 // Enable auto increment mode
                       | 0 << 4; // Enable the oscillator

    i2c->address(SHIELD_I2C_ADDR);
    i2c->write(writeBuf, 2);
}

void pwmUtils::writePWM(int index, double duty) {
  assert(0.0 <= duty && duty <= 1.0);
  assert(0 <= index && index < 16);
  if (duty>1 || duty<0 || index <0 || index >16){
      return;
  }
  double on = 4095.0 * duty;  // 4096-1
  uint16_t onRounded = (uint16_t) on; //Sketch way to round

  uint8_t writeBuf[5];

  // ON_L
  writeBuf[0] = registers[index];
  writeBuf[1] = 0x00; // ON LSB
  writeBuf[2] = 0x00; // ON MSB
  writeBuf[3] = onRounded & 0xFF; // OFF LSB
  writeBuf[4] = (onRounded >> 8) & 0xFF; // OFF MSB
  i2cPin.address(SHIELD_I2C_ADDR);
  i2cPin.write(writeBuf, 5);
}

//Why not? It is in the staff code anyway
void pwmUtils::setServoPosition(int index, double duty) {
  writePWM(index, .04 * duty + .04);
}
