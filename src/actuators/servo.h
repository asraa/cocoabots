#ifndef SERVO_H
#define SERVO_H
#include "mraa.hpp"

class servo
{
public:
    servo(int PWMindex);
    int servoIndex;
};

#endif // SERVO_H
