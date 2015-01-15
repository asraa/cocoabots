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
int running;
void stopMotors(int signo)
{
  if (signo == SIGINT) {
    actPointer->setPowerLeftWheel(0);
    actPointer->setPowerRightWheel(0);
    running =0;
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
        actuator myactuator;
        actPointer= &myactuator;
        sensorsModule mysensors;
        motorsControl control(&mysensors);
        control.desiredNormalizedAngularSpeed=0;
        control.desiredNormalizedSpeed=0;
        running=1;
        while(running)
        {
            myactuator.setPowerLeftWheel(control.leftMotorPower);
            myactuator.setPowerRightWheel(control.rightMotorPower);
            printf("time =%lld\n", mysensors.timeMicrosecondsSinceEpoch);
            printf("leftPower =%f\n", control.leftMotorPower);
            printf("rightPower =%f\n", control.rightMotorPower);
            printf("angle =%f\n", mysensors.gyroscopeAngle);
            printf("rightRotation =%f\n", mysensors.rightEncoderRotations );
            printf("leftRotation =%f\n", mysensors.leftEncoderRotations );

            usleep(200000.0);

        }
    }
    else if (strcmp(argv[1],"motorcrazy")==0){
        signal(SIGINT, stopMotors);
        actuator myactuator;
        running=1;
        while(running)
        {
            myactuator.setPowerLeftWheel(1);
            myactuator.setPowerRightWheel(1);


            usleep(200000.0);

        }
    }
  }
 
  return 0;
}


