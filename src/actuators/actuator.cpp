#include "actuator.h"

actuator::actuator():actuator(NULL){

}

actuator::actuator(sensorsModule * sensors):
    rightWheel(RIGHT_WHEEL_DIR, RIGHT_WHEEL_PWM),
    leftWheel(LEFT_WHEEL_DIR, LEFT_WHEEL_PWM),
    pwm(),
    sensorsPointer(sensors),
    sortServo(SORT_SERVO),
    armServo(ARM_SERVO),
    hookServo(HOOK_SERVO)
{
}


void actuator::setPowerLeftWheel(double speed){
    int dir;
    if (speed >=0){
        dir =MOTOR_DIRECTION_FRONT;
    } else {
        dir =MOTOR_DIRECTION_BACK;
        speed=-speed;
    }

#if LEFT_ENCODER
    if(sensorsPointer){
        sensorsPointer->leftEncoder.dir =dir;
    }
#endif

    leftWheel.dirPin.write(dir);
    speed = speed>1? 1: speed; //If speed >1, speed =1.
    speed= speed * MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS;
    pwm.writePWM(leftWheel.pwmIndex,speed);
}

void actuator::setPowerRightWheel(double speed){
    int dir;
#if MOTORS_OPPOSITE
    if (speed >=0){
        dir =MOTOR_DIRECTION_BACK;
    } else {
        dir =MOTOR_DIRECTION_FRONT;
        speed=-speed;
    }
#else
    if (speed >=0){
        dir =MOTOR_DIRECTION_FRONT;
    } else {
        dir =MOTOR_DIRECTION_BACK;
        speed=-speed;
    }
#endif

#if RIGHT_ENCODER
    if(sensorsPointer){
        sensorsPointer->rightEncoder.dir =dir;
    }
#endif
    rightWheel.dirPin.write(dir);
    speed = speed>1? 1: speed; //If speed >1, speed =1.
    speed= speed * MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS;
    pwm.writePWM(rightWheel.pwmIndex,speed);
}

void actuator::setSortServo(double angle){
    double duty = angle/180.0;
    pwm.setServoPosition(sortServo.servoIndex,duty);
}
void actuator::setArmServo(double angle){
    double duty = angle/180.0;
    pwm.setServoPosition(armServo.servoIndex,duty);
}
void actuator::setHookServo(double angle){
    double duty = angle/180.0;
    pwm.setServoPosition(hookServo.servoIndex,duty);
}

