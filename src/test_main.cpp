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


actuator * actPointer;
int RUNNING;
void stopMotors(int signo)
{
    if (signo == SIGINT) {
        RUNNING =0;
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
            control.desiredNormalizedAngularSpeed=0;
            control.desiredNormalizedSpeed=0;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);
                printf("time =%lld\n", mysensors.timeMicrosecondsSinceEpoch);
                printf("leftPower =%f\n", control.leftMotorPower);
                printf("rightPower =%f\n", control.rightMotorPower);
                printf("speed =%f\n", control.normalizedSpeed);
                printf("angularspeed =%f\n", control.normalizedAngularSpeed);
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
            control.desiredNormalizedAngularSpeed=0;
            control.desiredNormalizedSpeed=0.3;
            RUNNING=1;
            while(RUNNING)
            {
                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);
                printf("time =%lld\n", mysensors.timeMicrosecondsSinceEpoch);
                printf("leftPower =%f\n", control.leftMotorPower);
                printf("rightPower =%f\n", control.rightMotorPower);
                printf("speed =%f\n", control.normalizedSpeed);
                printf("angularspeed =%f\n", control.normalizedAngularSpeed);
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
            control.desiredNormalizedAngularSpeed=0;
            control.desiredNormalizedSpeed=0;
            RUNNING=1;
            while(RUNNING)
            {
                control.desiredNormalizedAngularSpeed= mysensors.gyroscopeAngle*-0.008;
                myactuator.setPowerLeftWheel(control.leftMotorPower);
                myactuator.setPowerRightWheel(control.rightMotorPower);


                usleep(50000.0);

            }
        }
        return 0;
    }
}


