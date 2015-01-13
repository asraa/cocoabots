#include "ultrasonic.h"
#include <unistd.h>
#include <sys/time.h>

ultrasonic::ultrasonic(int TP, int EP): trigGpio(TP), echoGpio(EP){
    trigGpio.dir(mraa::DIR_OUT);
    echoGpio.dir(mraa::DIR_IN);

    echoGpio.isr(mraa::EDGE_BOTH, echo_handler, this);
    running=1;
    runThread = new std::thread(run,this);
    //runThread->detach();

}

ultrasonic::~ultrasonic(){
    running=0;
    runThread->join();
    delete runThread;
}

void ultrasonic::echo_handler(void* ultrasonicSensorPointer) {
    // Grab end time first, for accuracy
    struct timeval end;
    gettimeofday(&end, NULL);

    ultrasonic *ultSensor = (ultrasonic *) ultrasonicSensorPointer;
    mraa::Gpio* echo = & (ultSensor->echoGpio) ;
    static struct timeval start;
    bool rising = echo->read() == 1;
    if (rising) {
        gettimeofday(&start, NULL);
    }
    else {
        int diffSec = end.tv_sec - start.tv_sec;
        //std::cout << "Diff sec: " << diffSec << std::endl;
        int diffUSec = end.tv_usec - start.tv_usec;
        //std::cout << "Diff usec: " << diffUSec << std::endl;
        double diffTime = (double)diffSec + 0.000001*diffUSec;
        //std::cout << "Diff time: " << diffTime << std::endl;
        // Speed of sound conversion: 340m/s * 0.5 (round trip)
        //std::cout << "Distance: " <<  diffTime * 170.0 << "m" << std::endl;
        ultSensor->duration=diffTime;
        ultSensor->distance_m = diffTime * 170.0;
        ultSensor->distance_in= diffTime * 170.0 *39.37;
    }

}

void ultrasonic::run(void * ultrasonicSensorPointer){
    ultrasonic * ultSens = (ultrasonic *)ultrasonicSensorPointer;
    while (ultSens->running){
        // 20us trigger pulse (must be at least 10us)
        ultSens->trigGpio.write(1);
        usleep(20);
        ultSens->trigGpio.write(0);

        // Must pause at least 60ms between measurements
        usleep(ULTRASONIC_UPDATE_TIME);
    }
}



double ultrasonic::getDistance(int sys){
    if (sys==ULTRASONIC_M){
        return distance_m;
    }else{
        return distance_in;
    }
}

double ultrasonic::getTiming(){
    return duration;
}
