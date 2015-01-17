#include "motorscontrol.h"
#include <unistd.h>
motorsControl::motorsControl(sensorsModule *sensors): mysensors(sensors)
{
    isTurning=0;
    fwdSpeedGain =FWD_SPEED_GAIN;
    fwdErrorGain =FWD_ERROR_GAIN;
    angSpeedGain = ANG_SPEED_GAIN;
    angErrorGain = ANG_ERROR_GAIN;
    previousAngle = mysensors->gyroscopeAngle;
    previousPosition = (mysensors->rightEncoderMovement+mysensors->leftEncoderMovement)/2;
    previousTime = (mysensors->timeMicrosecondsSinceEpoch);
    rightMotorPower = 0;
    leftMotorPower =0;
    previousLeftWheelPosition=0;
    previousRightWheelPosition=0;
    normalizedLeftWheelSpeed=0;
    normalizedRightWheelSpeed=0;

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
    double nextPosition= desiredPosition;

    double angSpeed = realAngularSpeed;
    if ((angSpeed < ANG_SPEED_TOLERANCE) &&(-angSpeed<ANG_SPEED_TOLERANCE)){
        angSpeed=0;
    }
    double realAngle = getNewAngle();
    int angError = getAngleError(desiredAngle,realAngle);
    double angCorrection = (angError*angErrorGain + angSpeed*angSpeedGain) * CLOCKWISE_POSITIVE;

    if (angCorrection >MAXIMUM_DYNAMIC_TURN_ANGLE){
        if (isTurning==0){
            positionStartTurning=getNewPosition();
            nextPosition=positionStartTurning;
            isTurning=1;
        }else{
            nextPosition=positionStartTurning;
        }
    }
    else{
        isTurning=0;
    }

    double fwdSpeed = realSpeed;
    if ((fwdSpeed < POSITION_SPEED_TOLERANCE) &&(-fwdSpeed<POSITION_SPEED_TOLERANCE)){
        fwdSpeed=0;
    }

    double fwdError = getPositionError(nextPosition,getNewPosition());
    double fwdCorrection = (fwdError * fwdErrorGain+ fwdSpeed*fwdSpeedGain);
    if (fwdCorrection>1){
        fwdCorrection=1;
    }else if (fwdCorrection<-1){
        fwdCorrection=-1;
    }



    double newRightMotorPower = fwdCorrection - angCorrection;
    double newLeftMotorPower = fwdCorrection + angCorrection;
    newRightMotorPower = currentLimiter(normalizedRightWheelSpeed,newRightMotorPower);
    newLeftMotorPower = currentLimiter(normalizedLeftWheelSpeed,newLeftMotorPower);
    newRightMotorPower = powerMinimumThreshold(newRightMotorPower);
    newLeftMotorPower = powerMinimumThreshold(newLeftMotorPower);
    rightMotorPower =newRightMotorPower;
    leftMotorPower = newLeftMotorPower;
}


double motorsControl::currentLimiter(double normalizedWheelSpeed, double power){
    if ((power>0 && normalizedWheelSpeed >=0)&&((power - normalizedWheelSpeed)>CURRENT_LIMIT)){
        return normalizedWheelSpeed+CURRENT_LIMIT;
    }
    else if ((power<0 && normalizedWheelSpeed <=0)&&((normalizedWheelSpeed - power)>CURRENT_LIMIT)){
        return normalizedWheelSpeed-CURRENT_LIMIT;
    }
    else if ((power<0 && normalizedWheelSpeed >=0)&&( power < -BACKWARDS_CURRENT_LIMIT)){
        return -BACKWARDS_CURRENT_LIMIT;
    }
    else if ((power>0 && normalizedWheelSpeed <=0)&&( power > BACKWARDS_CURRENT_LIMIT)){
        return BACKWARDS_CURRENT_LIMIT;
    }
    else {
        return power;
    }
}

int motorsControl::getAngleError(double desiredAngle, double realAngle){
    int angError = (int)(desiredAngle - realAngle);
    angError %=360;
    if (angError <-180){
        angError+=360;
    }
    if (angError > 180){
        angError-=360;
    }

    if ((angError<ANG_TOLERANCE)&&(-angError<ANG_TOLERANCE)){
        angError=0;
    }
    return angError;
}

double motorsControl::getPositionError(double desiredPosition, double realPosition){
    double fwdError = desiredPosition-realPosition;

    if ((fwdError< POSITION_TOLERANCE) && (-fwdError< POSITION_TOLERANCE)){
        fwdError=0;
    }
    return fwdError;
}

double motorsControl::powerMinimumThreshold(double power){
    if (power>0 && power<MINIMUM_THRESHOLD_PWM){
        power = MINIMUM_THRESHOLD_PWM;
    }
    else if (power<0 && -power<MINIMUM_THRESHOLD_PWM){
        power = -MINIMUM_THRESHOLD_PWM;
    }
    return power;
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
    return (mysensors->rightEncoderMovement+mysensors->leftEncoderMovement)/2;
}


double motorsControl::getNewAngle(){
#if USE_GIROSCOPE_FOR_ANGLE
    return getNewAngleFromGyroscope();
#else
    return getNewAngleFromEncoders();
#endif
}

double motorsControl::getNewAngleFromGyroscope(){
    return (mysensors->gyroscopeAngle);
}

double motorsControl::getNewAngleFromEncoders(){
    return (mysensors->encoderAngle);
}

double motorsControl::getNewLeftWheelPosition(){
    return mysensors->leftEncoderMovement;
}

double motorsControl::getNewRightWheelPosition(){
    return mysensors->rightEncoderMovement;

}

motorsControl::~motorsControl(){
    running=0;
    runThread->join();
    delete runThread;
}
