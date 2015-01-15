#include "actuator.h"

actuator::actuator():
    rightWheel(RIGHT_WHEEL_DIR, RIGHT_WHEEL_PWM),
    leftWheel(LEFT_WHEEL_DIR, LEFT_WHEEL_PWM),
    pwm()
{
}


void actuator::setPowerLeftWheel(double speed){
    int dir;
    if (speed >=0){
        dir =MOTOR_DIRECTION_LEFT;
    } else {
        dir =MOTOR_DIRECTION_RIGHT;
        speed=-speed;
    }
    leftWheel.dirPin.write(dir);
    pwm.writePWM(leftWheel.pwmIndex,speed);
}

void actuator::setPowerRightWheel(double speed){
    int dir;
    if (speed >=0){
        dir =MOTOR_DIRECTION_RIGHT;
    } else {
        dir =MOTOR_DIRECTION_LEFT;
        speed=-speed;
    }
    rightWheel.dirPin.write(dir);
    pwm.writePWM(rightWheel.pwmIndex,speed);
}
