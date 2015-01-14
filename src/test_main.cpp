#include <cstdio>
#include "cocoabot.h"
#include <cstring>
#include "sensors/ultrasonic.h"
#include "actuators/pwmutils.h"
#include "actuators/pid.h"

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
            usleep(200000.0);
            pwm.setServoPosition(0,1);
            usleep(200000.0);
            pwm.setServoPosition(0,2);
            usleep(200000.0);
            pwm.setServoPosition(0,3);
            usleep(200000.0);
            pwm.setServoPosition(0,20);



        }
    }

    else if (strcmp(argv[1],"ultrasonicpidmotor")==0){
        //test out pid control to make motors move to a fixed distance from wall, feedback with ultrasonic
        ultrasonic testUltrasonic(2,3);
        pwmUtils pwm;
        pid testPID(1,0,0);

        while(1){
            int targetDist = 10; //get 10 cm away
            pwm.writePWM(1,testPID.calcPID(targetDist,testUltrasonic.getDistance(1))); //first motor
            pwm.writePWM(2,testPID.calcPID(targetDist,testUltrasonic.getDistance(1))); //second motor


        }
    }
  }
 
  return 0;
}
