#include <cstdio>
#include "cocoabot.h"
#include <cstring>
#include <cmath>
#include "sensors/ultrasonic.h"
#include "actuators/pwmutils.h"
#include "actuators/pid.h"
#include "actuators/actuator.h"
#include "sensorsmodule.h"
#include "actuators/motorscontrol.h"
#include "mraa.hpp"
#include <signal.h>
#include  <thread>



actuator * actPointer;
int RUNNING;
void stopMotors(int signo)
{
    if (signo == SIGINT) {
        RUNNING =0;
        double a =0;
        actPointer->leftWheelPower = &a;
        actPointer->rightWheelPower =&a;
        actPointer->setPowerLeftWheel(0);
        actPointer->setPowerRightWheel(0);
        RUNNING =0;
    }
}

int main(int argc, char** argv){

    //if no parameter was passed
    if (argc==1){
        printf("Missing Arguments\n The usage is: test_run arguments");
    }
    else{
        if(strcmp(argv[1],"ultrasonic")==0){
            printf("Running ultrasonic test\n");
            ultrasonic testUltrasonic(2,3);
            while(1)
            {
                usleep(200000.0);
                printf("%f\n", testUltrasonic.getDistance(0));
            }

        }
        else if (strcmp(argv[1],"ultrasonicpwm")==0){
            ultrasonic testUltrasonic(2,3);
            pwmUtils pwm;
            //We have a servo in 0
            while(1)
            {
                pwm.setServoPosition(0,testUltrasonic.getDistance(0)/40.0);


            }
        }

        else if (strcmp(argv[1],"pwmservo")==0){
            pwmUtils pwm;
            //We have a servo in 0
            while(1)
            {
                usleep(200000.0);
                pwm.setServoPosition(0,0.3);
                usleep(200000.0);
                pwm.setServoPosition(0,0.5);
            }
        }
        else if (strcmp(argv[1],"sensormoduleultrasonic")==0){
            sensorsModule mysensors;
            while(1)
            {
                printf("%f\n", mysensors.frontUltrasonicData);
                usleep(200000.0);

            }
        }

        else if (strcmp(argv[1],"ultrasonicpidmotor")==0){
            //test out pid control to make motors move to a fixed distance from wall, feedback with ultrasonic
            ultrasonic testUltrasonic(2,3);
            pwmUtils pwm;
            pid testPID(1,0,0);
            mraa::Gpio dirPin1(8);
            mraa::Gpio dirPin2(9);

            while(1){
                int targetDist = 10; //get 10 cm away
                float update = testPID.calcPID(targetDist,testUltrasonic.getDistance(1));
                pwm.writePWM(1,std::abs(update)); //first motor PWM pin 1
                if (update < 0){
                    dirPin1.write(0);
                    dirPin2.write(1);
                }
                else {
                    dirPin1.write(1);
                    dirPin2.write(0);
                }

                //first motor dir pin 8
                printf("%f\n", update);
                pwm.writePWM(2,std::abs(update)); //second motor PWM pin 2
                //second motor dir pin 9

            }
        }
        else if (strcmp(argv[1],"testtime")==0){
            sensorsModule mysensors;
            while(1)
            {
                printf("%lld\n", mysensors.timeMicrosecondsSinceEpoch);
                usleep(200000.0);

            }
        }
        else if (strcmp(argv[1],"straightline")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=1;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);
                printf("time =%lld\n", mysensors.timeMicrosecondsSinceEpoch);
                printf("leftPower =%f\n", control.leftMotorPower);
                printf("rightPower =%f\n", control.rightMotorPower);
                printf("speed =%f\n", control.realSpeed);
                printf("angularspeed =%f\n", control.realAngularSpeed);
                printf("angle =%f\n", mysensors.gyroscopeAngle);
                printf("rightRotation =%f\n", mysensors.rightEncoderRotations );
                printf("leftRotation =%f\n", mysensors.leftEncoderRotations );
                printf("control Position =%f\n", control.previousPosition );
                printf("control Angle =%f\n", control.previousAngle);
                printf("control time =%f\n", control.previousTime);


                usleep(200000.0);

            }
        }
        else if (strcmp(argv[1],"straightline2")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle =90 ;
            control.desiredPosition=1;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);
                printf("time =%lld\n", mysensors.timeMicrosecondsSinceEpoch);
                printf("leftPower =%f\n", control.leftMotorPower);
                printf("rightPower =%f\n", control.rightMotorPower);
                printf("speed =%f\n", control.realSpeed);
                printf("angularspeed =%f\n", control.realAngularSpeed);
                printf("angle =%f\n", mysensors.gyroscopeAngle);
                printf("rightRotation =%f\n", mysensors.rightEncoderRotations );
                printf("leftRotation =%f\n", mysensors.leftEncoderRotations );
                printf("control Position =%f\n", control.previousPosition );
                printf("control Angle =%f\n", control.previousAngle);
                printf("control time =%f\n", control.previousTime);


                usleep(50000.0);

            }
        }
        else if (strcmp(argv[1],"motorcrazy")==0){
            signal(SIGINT, stopMotors);
            actuator myactuator;
            actPointer = &myactuator;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(1);
                myactuator.setPowerRightWheel(1);


                usleep(200000.0);

            }
        }
        else if (strcmp(argv[1],"motorcrazy2")==0){
            signal(SIGINT, stopMotors);
            actuator myactuator;
            actPointer = &myactuator;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(-1);
                myactuator.setPowerRightWheel(-1);


                usleep(200000.0);
                myactuator.setPowerLeftWheel(1);
                myactuator.setPowerRightWheel(1);
                usleep(200000.0);
            }
        }
        else if (strcmp(argv[1],"motorstop")==0){
            signal(SIGINT, stopMotors);
            actuator myactuator;
            actPointer = &myactuator;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(0);
                myactuator.setPowerRightWheel(0);


                usleep(200000.0);

            }
        }

        else if (strcmp(argv[1],"encoder")==0){
            encoder myenc(RIGHT_ENCODER_ENC);
            while (1){
                usleep(200000.0);
                printf("right encoder counts =%lld\n", myenc.edgeCount);


            }
        }
        else if (strcmp(argv[1],"stayStraight")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);


                usleep(10000.0);

            }
        }
        else if (strcmp(argv[1],"stayStraightDebug")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            RUNNING=1;
            while(RUNNING)
            {
                printf("leftWheelSpeed =%f and power =%f\n", control.normalizedLeftWheelSpeed ,control.leftMotorPower );
                printf("rightWheelSpeed =%f and power =%f\n", control.normalizedRightWheelSpeed ,control.rightMotorPower );

                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);


                usleep(100000.0);

            }
        }
        else if (strcmp(argv[1],"findProportionalGain")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            double proportionalGain =0.001;
            control.angSpeedGain=0;
            control.angErrorGain = proportionalGain;
            control.fwdErrorGain =0;
            control.fwdSpeedGain =0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {
                control.angErrorGain = proportionalGain;
                printf("proportionalGain =%f\n", proportionalGain);
                control.desiredAngle=90;
                usleep(3000000.0);
                control.desiredAngle=0;
                proportionalGain +=0.001;
                printf("proportionalGain =%f\n", proportionalGain);
                control.angErrorGain = proportionalGain;
                usleep(3000000.0);
                proportionalGain +=0.001;


            }
        }
        else if (strcmp(argv[1],"findDerivativeGain")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            double derivativeGain =-0.0001;
            control.angSpeedGain=derivativeGain;
            control.fwdErrorGain =0;
            control.fwdSpeedGain =0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {
                control.angSpeedGain = derivativeGain;
                printf("derivativeGain =%f\n", derivativeGain);
                control.desiredAngle=90;
                usleep(3000000.0);
                control.desiredAngle=0;
                derivativeGain +=-0.0001;
                printf("derivativeGain =%f\n", derivativeGain);
                control.angSpeedGain = derivativeGain;
                usleep(3000000.0);
                derivativeGain +=-0.0001;


            }
        }
        else if (strcmp(argv[1],"fwdGain")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            double fwdGain =0.01;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {
                control.fwdErrorGain = fwdGain;
                printf("fwdGain =%f\n", fwdGain);
                control.desiredPosition=2.5;
                usleep(3000000.0);
                control.desiredPosition=0;
                fwdGain +=0.01;
                printf("fwdGain =%f\n", fwdGain);
                control.fwdErrorGain = fwdGain;
                usleep(3000000.0);
                fwdGain +=0.01;


            }
        }
        else if (strcmp(argv[1],"square")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            double desiredPos;
            desiredPos =0;
            double desiredAngle =90;
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {

                control.desiredPosition+=1.5;
                control.desiredAngle+=90;
                usleep(3000000.0);


            }
        }
        else if (strcmp(argv[1],"ultrasonicPing")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            while(RUNNING)
            {
                if(control.previousPosition-control.desiredPosition<0.5){
                    control.desiredPosition+=1;
                }
                if (mysensors.frontUltrasonicData < 30){
                    control.desiredAngle+=90;
                }
                usleep(2000000.0);


            }
        }
        else if (strcmp(argv[1],"ultrasonicPingState")==0){
            signal(SIGINT, stopMotors);
            sensorsModule mysensors;
            actuator myactuator(&mysensors);
            actPointer= &myactuator;
            motorsControl control(&mysensors);
            control.desiredAngle=0;
            control.desiredPosition=0;
            myactuator.leftWheelPower = &control.leftMotorPower;
            myactuator.rightWheelPower= &control.rightMotorPower;
            RUNNING =1;
            int state=1;
            while(RUNNING)
            {
                if (state){
                    if(control.previousPosition-control.desiredPosition<0.5){
                        control.desiredPosition+=1;
                    }
                    if (mysensors.frontUltrasonicData < 30){
                        control.desiredAngle+=90;
                        control.desiredPosition=0;
                        state=0;
                    }
                }
                else{
                    double angleError = control.getAngleError(control.previousAngle,control.desiredAngle);
                    if (angleError<0){
                        angleError*=-1;
                    }
                    if ( angleError <7) {
                        state=1;
                    }
                }
                usleep(20000.0);


            }
        }
        return 0;
    }
}


