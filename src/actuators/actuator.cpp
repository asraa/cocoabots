#include "actuator.h"
#include <unistd.h>

actuator::actuator():
    rightWheel(RIGHT_WHEEL_DIR, RIGHT_WHEEL_PWM),
    leftWheel(LEFT_WHEEL_DIR, LEFT_WHEEL_PWM),
    pwm(),
    sortServo(SORT_SERVO_PWM),
    armServo(ARM_SERVO_PWM),
    hookServo(HOOK_SERVO_PWM),
    rightWheelPower(NULL),
    leftWheelPower(NULL),
    armServoAngle(NULL),
    hookServoAngle(NULL),
    sortServoAngle(NULL),
    sensorsPointer(NULL){

    running=1;
    runThread = new std::thread(run,this);


}

actuator::actuator(motorsControl &mymotorsControl, servosControl &myservosControl):actuator()
    {
    rightWheelPower = &mymotorsControl.rightMotorPower;
    leftWheelPower = &mymotorsControl.leftMotorPower;
    armServoAngle = &myservosControl.armAngle;
    hookServoAngle = &myservosControl.hookAngle;
    sortServoAngle = &myservosControl.sortAngle;

}
//The actuator should receive an instance of sensors, so it can update the dir pin
// of the encoder. //OBSOLETE
actuator::actuator(sensorsModule * sensors):actuator()
{
    sensorsPointer= sensors;
    //It starts its own thread responsible for writting to the motors and servos.

}
actuator::~actuator(){
    running=0;
    runThread->join();
    delete runThread;
}

void actuator::run(actuator * myactuator){

    //Here we see for each of the powers/angles if they have been defined
    //i.e. they point to a value
    // if they do, we update the things.
    while (myactuator->running){
        if (myactuator->rightWheelPower)
            myactuator->setPowerRightWheel(*(myactuator->rightWheelPower));
        if (myactuator->leftWheelPower)
            myactuator->setPowerLeftWheel(*(myactuator->leftWheelPower));
        if (myactuator->armServoAngle)
           myactuator->setArmServoAngle(*(myactuator->armServoAngle));
        if (myactuator->hookServoAngle)
            myactuator->setHookServoAngle(*(myactuator->hookServoAngle));
        if (myactuator->sortServoAngle)
            myactuator->setSortServoAngle(*(myactuator->sortServoAngle));
        usleep(UPDATE_RATE_ACTUATORS_MILISECONDS * 1000);
    }

}


void actuator::setPowerLeftWheel(double speed){
    int dir;
    int encoderDir;
    if (speed >=0){
        encoderDir=0;
        dir =MOTOR_DIRECTION_FRONT;
    } else {
        encoderDir=1;
        dir =MOTOR_DIRECTION_BACK;
        speed=-speed;
    }

#if LEFT_ENCODER
    if(sensorsPointer){
        sensorsPointer->leftEncoder.dir =encoderDir;
    }
#endif

    leftWheel.dirPin.write(dir);
    speed = speed>1? 1: speed; //If speed >1, speed =1.
    speed= speed * MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS;
    pwm.writePWM(leftWheel.pwmIndex,speed);
}

void actuator::setPowerRightWheel(double speed){
    int dir;
    int encoderDir;
#if MOTORS_OPPOSITE
    if (speed >=0){
        dir =MOTOR_DIRECTION_BACK;
        encoderDir=0;
    } else {
        encoderDir=1;
        dir =MOTOR_DIRECTION_FRONT;
        speed=-speed;
    }
#else
    if (speed >=0){
        encoderDir=0;
        dir =MOTOR_DIRECTION_FRONT;
    } else {
        encoderDir=1;
        dir =MOTOR_DIRECTION_BACK;
        speed=-speed;
    }
#endif

#if RIGHT_ENCODER
    if(sensorsPointer){
        sensorsPointer->rightEncoder.dir =encoderDir;
    }
#endif
    rightWheel.dirPin.write(dir);
    speed = speed>1? 1: speed; //If speed >1, speed =1.
    speed= speed * MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS;
    pwm.writePWM(rightWheel.pwmIndex,speed);
}

void actuator::setSortServoAngle(double angle){
    double duty = angle/180.0;
    pwm.setServoPosition(sortServo.servoIndex,duty);

}

double actuator::getSortServoAngle(){
    if (sortServoAngle){
    return *sortServoAngle;}
    return -1;
}

void actuator::setArmServoAngle(double angle){
    double duty = angle/180.0;
    pwm.setServoPosition(armServo.servoIndex,duty);

}

double actuator::getArmServoAngle(){
    if (armServoAngle){
    return *armServoAngle;}
    return -1;
}

void actuator::setHookServoAngle(double angle){
    double duty = angle/180.0;
    pwm.setServoPosition(hookServo.servoIndex,duty);

}

double actuator::getHookServoAngle(){
    if (hookServoAngle){
    return *hookServoAngle;}
    return -1;
}

