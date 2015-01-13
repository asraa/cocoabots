#include <cstdio>
#include "cocoabot.h"
#include <cstring>
#include "sensors/ultrasonic.h"

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
            printf("%d\n", testUltrasonic.getDistance(0));
        }

    }
    else if (strcmp(argv[1],"rotate")==0){
      //rotateTest();
    }

    else if (strcmp(argv[1],"moveToWall")==0){
      //moveToWallTest();
    }    
  }
 
  return 0;
}
