#include "motorscontrol.h"
#include <unistd.h>

motorsControl::motorsControl(sensorsModule *sensors)
{
    fwdSpeedGain =FWD_SPEED_GAIN;
    fwdErrorGain =FWD_ERROR_GAIN;
    angSpeedGain = ANG_SPEED_GAIN;
    angErrorGain = ANG_ERROR_GAIN;
    mysensors=sensors;
    previousAngle = mysensors->gyroscopeAngle;
    previousPosition = (mysensors->rightEncoderRotations+mysensors->leftEncoderRotations)/2;
    previousTime = (mysensors->timeMicrosecondsSinceEpoch);
    rightMotorPower = 0;
    leftMotorPower =0;
    previousLeftWheelPosition=0;
    previousRightWheelPosition=0;
    normalizedLeftWheelSpeed=0;
    normalizedRightWheelSpeed;

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
    updateWheelsSpeed();

    updatePosition();
    updateAngle();
    updateWheelsPositions();

    updateTime();

    double fwdSpeed = realSpeed;
    double fwdError = desiredPosition-getNewPosition();
    double fwdCorrection = (fwdError * fwdErrorGain+ fwdSpeed*fwdSpeedGain);

    double angSpeed = realAngularSpeed;
    int angError = desiredAngle - getNewAngle(); //desiredNormalizedAngularSpeed - angSpeed;
    angError %=360;
    if (angError > 180){
        angError-=360;
    }
    double angCorrection = (angError*angErrorGain + angSpeed*angSpeedGain) * GYROSCOPE_CLOCKWISE_POSITIVE;

    double newRightMotorPower = fwdCorrection - angCorrection; // + rightMotorPower
    double newLeftMotorPower = fwdCorrection + angCorrection; // + leftMotorPower
    newRightMotorPower = currentLimiter(normalizedRightWheelSpeed,newRightMotorPower);
    newLeftMotorPower = currentLimiter(normalizedLeftWheelSpeed,newRightMotorPower);
    rightMotorPower =newRightMotorPower;
    leftMotorPower = newLeftMotorPower;
}

double motorsControl::currentLimiter(double normalizedWheelSpeed, double power){
    if ((power - normalizedWheelSpeed)>CURRENT_LIMIT){
        return CURRENT_LIMIT;
    }
    else if ((normalizedWheelSpeed - power)>CURRENT_LIMIT){
        return -CURRENT_LIMIT;
    }
    else {
        return power;
    }
}

void motorsControl::updateSpeed(){
    double newPosition =getNewPosition();
    double timeMicroSeconds = mysensors->timeMicrosecondsSinceEpoch;
    double dt = timeMicroSeconds-previousTime;
    double newSpeed = (newPosition -previousPosition)*MICROSECOND/dt;
    realSpeed=newSpeed;
    //normalizedSpeed=realSpeed/MAXIMUM_SPEED;
}

void motorsControl::updateAngularSpeed(){
    double newAngle =getNewAngle();
    double timeMicroSeconds = mysensors->timeMicrosecondsSinceEpoch;
    double dt = timeMicroSeconds-previousTime;
    double newAngularSpeed = (newAngle -previousAngle)*MICROSECOND/dt;
    realAngularSpeed=newAngularSpeed;
    //normalizedAngularSpeed=realAngularSpeed/MAXIMUM_ANGULAR_SPEED;
}

void motorsControl::updateWheelsSpeed(){
    double newRightWheelPosition = getNewRightWheelPosition();
    double newLeftWheelPosition = getNewLeftWheelPosition();
    double timeMicroSeconds = mysensors->timeMicrosecondsSinceEpoch;
    double dt = timeMicroSeconds-previousTime;
    double newRightWheelSpeed = (newRightWheelPosition-previousRightWheelPosition)*MICROSECOND/dt;
    double newLeftWheelSpeed = (newLeftWheelPosition-previousLeftWheelPosition)*MICROSECOND/dt;
    normalizedLeftWheelSpeed = newLeftWheelSpeed/MAXIMUM_REVOLUTIONS_PER_SECOND;
    normalizedRightWheelSpeed = newRightWheelSpeed/MAXIMUM_REVOLUTIONS_PER_SECOND;
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

void motorsControl::updateWheelsPositions(){
    previousRightWheelPosition = getNewRightWheelPosition();
    previousLeftWheelPosition = getNewLeftWheelPosition();
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


double motorsControl::getNewLeftWheelPosition(){
    return mysensors->leftEncoderRotations;
}

double motorsControl::getNewRightWheelPosition(){
#if MOTORS_OPPOSITE
    return -mysensors->rightEncoderRotations;
#else
    return mysensors->rightEncoderRotations;
#endif

}

motorsControl::~motorsControl(){
    running=0;
    runThread->join();
    delete runThread;
}
