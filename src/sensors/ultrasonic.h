#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define ULTRASONIC_M 1
#define ULTRASONIC_INC 0
#define ULTRASONIC_UPDATE_TIME 60000

#include <sys/time.h>
#include "mraa.hpp"
#include  <thread>


//There is no assignment operator for this class.
class ultrasonic
{
  public:
    ultrasonic(int TP, int EP);
    ~ultrasonic();
    double getTiming();
    double getDistance(int sys);
    static void run(void * ultrasonicSensorPointer);
    static void echo_handler(void* ultrasonicSensorPointer);

    std::thread *runThread;
    mraa::Gpio trigGpio;
    mraa::Gpio echoGpio;
    double duration;
    double distance_m;
    double distance_in;
    int running;
    int trigPinN, echoPinN;

  private:

};

#endif

