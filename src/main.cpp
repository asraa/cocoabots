#include "cocoabot.h"
int main(int argc, char** argv){
  cocoabot mycocoabot();
  //if no parameter was passed
  if (argc==1){
      cocoabot.running=1;
      cocoabot.run();

  }
  else{
      //cocoabot mycocoabot(argc, argv); //returns after the end of the test.
  }
 
  return 0;
}


