#include "pid.h"
#include <unistd.h>
#include <sys/time.h>
#include <cmath>

float pid::calcPID(float desired, float actual){
    //ERROR
    float error = desired - actual;
    //INTEGRAL: initialize static inegral, so that it accumulates
    static float integral = 0;
    //if error's not that big, then don't integrate
    if (std::abs(error) > epsilon){
        integral += error*dt;
    }
    //DERIVATIVE, need previous error
    static float previousError = 0;
    float derivative = (error - previousError)/dt;

    float total = kp*error + ki*integral + kd*derivative;
    previousError = error; //set previous error
    return total;
}
