#include "mraa.hpp"
#include "src/sensors/ultrasonic.h"
#include <unistd.h>
#include <sys/time.h>
#include <iostream>

using namespace std;

int running = 1;

ultrasonic::ultrasonic(int TP, int EP)
{
  //!
  //! I'm unsure if we need to set owner=False for mraa::Gpio
  //! See: http://iotdk.intel.com/docs/master/mraa/classmraa_1_1_gpio.html#a43a6f9ce400074a7a09a3a3805d738f4
  //!

  // Eql to pinMode(TP,OUTPUT)
  //mraa::Gpio* TP_gpio = new mraa::Gpio(TP);
  //if (TP_gpio == NULL) {
  //  return MRAA_ERROR_UNSPECIFIED;
  //}
  //mraa_result_t TP_response = TP_gpio->dir(mraa::DIR_OUT);
  //if (TP_response != MRAA_SUCCESS) {
  //  mraa::printError(TP_response);
  //  return 1;
 // }

  // Eql to pinMode(EP,INPUT)
  //mraa::Gpio* EP_gpio = new mraa::Gpio(EP);
  //if (EP_gpio == NULL) {
  //  return MRAA_ERROR_UNSPECIFIED;
  //}
  //mraa_result_t EP_response = EP_gpio->dir(mraa::DIR_IN);
  //if (EP_response != MRAA_SUCCESS) {
  //  mraa::printError(EP_response);
  //  return 1;
  //}
  
  Trig_pin = TP;
  Echo_pin = EP;
}

void ultrasonic::timing()
{
  struct timeval start, end;

  mraa::Gpio* TP_gpio = new mraa::Gpio(Trig_pin);
  mraa::Gpio* EP_gpio = new mraa::Gpio(Echo_pin);

  TP_gpio->dir(mraa::DIR_OUT);
  EP_gpio->dir(mraa::DIR_IN);

  //EP_gpio->isr(mraa::EDGE_BOTH, echo_handler, 0);
  TP_gpio->write(0);
  usleep(2);
  TP_gpio->write(1);
  usleep(10);
  TP_gpio->write(0);

  while (EP_gpio->read() == 0) {
    gettimeofday(&start, NULL);
  }
  while (EP_gpio->read() == 1) {
    gettimeofday(&end, NULL);
  }

  int diffSec = end.tv_sec - start.tv_sec;
  int diffUsec = end.tv_usec - start.tv_usec;

  duration = (long)diffSec + 0.000001*diffUsec;
}

long ultrasonic::ranging(int sys)
{
  timing();
  distance_cm = duration /29 / 2 ;
  distance_in = duration / 74 / 2;
  if (sys) {
  	return distance_cm;
  }
  else {
  	return distance_in;
  }
}

// void ultrasonic::echo_handler(int Echo_pin, ) {
//   gettimeofday(&end, NULL);

//   mraa::Gpio* echo = new mraa::Gpio(Echo_pin);
//   echo->dir(mraa::DIR_IN);

//   bool rising = (echo->read() == 1);
//   if (rising) {
//     gettimeofday(&start, NULL);
//   }
//   else {
//     int diffSec = end.tv_sec - start.tv_sec;
//     int diffUsec = end.tv_usec - start.tv_usec;

//     duration = (long)diffSec + 0.000001*diffUsec;
//   }
// }

int main() {
	ultrasonic us_device(2,3);
	while (running) {
		usleep(100000);
		std::cout << us_device.ranging(0) << std::endl;
	}
  return 0;
}
