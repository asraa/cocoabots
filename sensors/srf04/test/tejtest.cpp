// Compile with:
// g++ test_ultrasonic.cpp -o test_ultrasonic -lmraa
// Accurate within 0.2m - 1.0m approximately.
#include <csignal>
#include <iostream>
#include <sys/time.h>
#include "mraa.hpp"
int running = 1;
void sig_handler(int signo)
{
  if (signo == SIGINT) {
    printf("closing spi nicely\n");
    running = 0;
  }
}
void echo_handler(void* args) {
  // Grab end time first, for accuracy
  struct timeval end;
  gettimeofday(&end, NULL);
  mraa::Gpio* echo = (mraa::Gpio*)args;
  static struct timeval start;
  bool rising = echo->read() == 1;
  if (rising) {
    gettimeofday(&start, NULL);
  }
  else {
    int diffSec = end.tv_sec - start.tv_sec;
    std::cout << "Diff sec: " << diffSec << std::endl;
    int diffUSec = end.tv_usec - start.tv_usec;
    std::cout << "Diff usec: " << diffUSec << std::endl;
    double diffTime = (double)diffSec + 0.000001*diffUSec;
    std::cout << "Diff time: " << diffTime << std::endl;
    // Speed of sound conversion: 340m/s * 0.5 (round trip)
    std::cout << "Distance: " << diffTime * 170.0 << "m" << std::endl;
  }
}
int main() {
  // Handle Ctrl-C quit
  signal(SIGINT, sig_handler);
  mraa::Gpio* trig = new mraa::Gpio(20);
  trig->dir(mraa::DIR_OUT);
  mraa::Gpio* echo = new mraa::Gpio(13);
  echo->dir(mraa::DIR_IN);
  // Set the echo handlers to receive rising or falling edges of the
  // echo pulse
  echo->isr(mraa::EDGE_BOTH, echo_handler, echo);
  while (running) {
    // 20us trigger pulse (must be at least 10us)
    trig->write(1);
    usleep(20);
    trig->write(0);
    // Must pause at least 60ms between measurements
    usleep(500000.0);
  }
}
