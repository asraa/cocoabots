#include <cstdio>
#include "cocoabot.h"
#include <cstring>
#include "sensors/

int main(int argc, char** argv){
  
  //if no parameter was passed
  if (argc==1){
    printf("Missing Arguments\n The usage is: test_run arguments");
  }
  else{
    if(strcmp(argv[1],"ultrassonic")==0){
      // wallFollowTest();
        //run Custom test
        printf("Running test\n");

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