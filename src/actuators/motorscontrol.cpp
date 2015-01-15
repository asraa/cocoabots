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


    running=1;
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
    double newPosition =(mysensors->rightEncoderRotations+mysensors->leftEncoderRotations)/2;
    double timeMicroSeconds = mysensors->timeMicrosecondsSinceEpoch;
    double newSpeed = (newPosition -previousPosition);//*MICROSECOND/timeMicroSeconds;
    realSpeed=newSpeed;
    normalizedSpeed=realSpeed/MAXIMUM_SPEED;
}

void motorsControl::updateAngularSpeed(){
    double newAngle =(mysensors->gyroscopeAngle);
    double timeMicroSeconds = mysensors->timeMicrosecondsSinceEpoch;
    double newAngularSpeed = (newAngle -previousAngle)*MICROSECOND/timeMicroSeconds;
    realAngularSpeed=newAngularSpeed;
    normalizedAngularSpeed=realAngularSpeed/MAXIMUM_ANGULAR_SPEED;
}

void motorsControl::updateTime(){
    previousTime=mysensors->timeMicrosecondsSinceEpoch;
}

void motorsControl::updatePosition(){
    previousPosition= (mysensors->rightEncoderRotations+mysensors->leftEncoderRotations)/2;
}

void motorsControl::updateAngle(){
    previousAngle=(mysensors->gyroscopeAngle);
}

motorsControl::~motorsControl(){
    running=0;
    runThread->join();
    delete runThread;
}
