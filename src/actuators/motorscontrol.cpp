#include "motorscontrol.h"
#include <unistd.h>

motorsControl::motorsControl(sensorsModule *sensors)
{
    mysensors=sensors;
    previousAngle = mysensors->gyroscopeAngle;
    previousPosition = (mysensors->rightEncoderRotations+mysensors->leftEncoderRotations)/2;
    previousTime = (mysensors->timeMicrosecondsSinceEpoch);
    rightMotorPower = 0;
    leftMotorPower =0;


    this->running=1;
    runThread = new std::thread(run,this);
}


void motorsControl::run(motorsControl *mycontrol){
    while (mycontrol->running) {
        usleep(SPEED_CONTROL_UPDATE_RATE_MILISECONDS*1000);
        mycontrol->computeNewMotorPowers();
    }
}

void motorsControl::computeNewMotorPowers(){
    updateSpeed();
    updateAngularSpeed();
    updatePosition();
    updateAngle();
    updateTime();

    float fwdSpeed = normalizedSpeed;
    float fwdError = desiredNormalizedSpeed-normalizedSpeed;
    float fwdCorrection = fwdError * FWD_SPEED_GAIN;

    float angSpeed = normalizedAngularSpeed;
    float angError = desiredNormalizedAngularSpeed - normalizedAngularSpeed;
    float angCorrection = angError*ANG_SPEED_GAIN * GYROSCOPE_CLOCKWISE_POSITIVE;

    float newRightMotorPower = rightMotorPower+ fwdCorrection - angCorrection;
    float newLeftMotorPower = leftMotorPower + fwdCorrection + angCorrection;
    rightMotorPower =newRightMotorPower;
    leftMotorPower = newLeftMotorPower;
}

void motorsControl::updateSpeed(){
    double newPosition =getNewPosition();
    double timeMicroSeconds = mysensors->timeMicrosecondsSinceEpoch;
    double dt = timeMicroSeconds-previousTime;
    double newSpeed = (newPosition -previousPosition)*MICROSECOND/dt;
    realSpeed=newSpeed;
    normalizedSpeed=realSpeed/MAXIMUM_SPEED;
}

void motorsControl::updateAngularSpeed(){
    double newAngle =getNewAngle();
    double timeMicroSeconds = mysensors->timeMicrosecondsSinceEpoch;
    double dt = timeMicroSeconds-previousTime;
    double newAngularSpeed = (newAngle -previousAngle)*MICROSECOND/dt;
    realAngularSpeed=newAngularSpeed;
    normalizedAngularSpeed=realAngularSpeed/MAXIMUM_ANGULAR_SPEED;
}

void motorsControl::updateTime(){
    previousTime=mysensors->timeMicrosecondsSinceEpoch;
}

void motorsControl::updatePosition(){
    previousPosition= getNewPosition();
}

void motorsControl::updateAngle(){
    previousAngle=getNewAngle();
}

double motorsControl::getNewPosition(){
    #if MOTORS_OPPOSITE
    return (-mysensors->rightEncoderRotations+mysensors->leftEncoderRotations)/2;
    #else
    return (mysensors->rightEncoderRotations+mysensors->leftEncoderRotations)/2;
    #endif
}


double motorsControl::getNewAngle(){
    return (mysensors->gyroscopeAngle);
}


motorsControl::~motorsControl(){
    running=0;
    runThread->join();
    delete runThread;
}
