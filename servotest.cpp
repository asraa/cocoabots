// Compile with:
// g++ test_servo_shield.cpp -o test_servo_shield -lmraa
// Controls a servo on port 0 of the servo shield.

#include "mraa.hpp"
#include <cassert>
#include <csignal>
#include <iostream>

#define SHIELD_I2C_ADDR 0x40

int running = 1;
#define MS 1000

uint8_t registers[] = {
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
  

void sig_handler(int signo)
{
  if (signo == SIGINT) {
    printf("closing spi nicely\n");
    running = 0;
  }
}

void initPWM(mraa::I2c *i2c) {
  char writeBuf[2] = {0};
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


void writePWM(mraa::I2c* i2c, int index, double duty) {
  assert(0.0 <= duty && duty <= 1.0);
  assert(0 <= index && index < 16);
  double on = 4095.0 * duty;
  uint16_t onRounded = (uint16_t) on;

  char writeBuf[5];

  // ON_L
  writeBuf[0] = registers[index];
  writeBuf[1] = 0x00; // ON LSB
  writeBuf[2] = 0x00; // ON MSB
  writeBuf[3] = onRounded & 0xFF; // OFF LSB
  writeBuf[4] = (onRounded >> 8) & 0xFF; // OFF MSB
  i2c->address(SHIELD_I2C_ADDR);
  i2c->write(writeBuf, 5);
}


void setServoPosition(mraa::I2c *i2c, int index, double duty) {
  writePWM(i2c, index, .04 * duty + .04);
}

int main()
{
  // Handle Ctrl-C quit
  signal(SIGINT, sig_handler);

  // Edison i2c bus is 6
  mraa::I2c* i2c = new mraa::I2c(6);
  assert(i2c != NULL);

  initPWM(i2c);

  while (running) {
    // Alternate two locations with 2-sec delay
    setServoPosition(i2c, 0, 0.2);
    sleep(2.0);
    setServoPosition(i2c, 0, 0.8);
    sleep(2.0);
  }
}
