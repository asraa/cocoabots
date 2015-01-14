#include <cstdio>
#include "cocoabot.h"
#include <cstring>
#include "sensors/ultrasonic.h"
#include "actuators/pwmutils.h"
#include "sensorsmodule.h"

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
  }
 
  return 0;
}
