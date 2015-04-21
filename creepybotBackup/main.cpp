#include "cocoabot.h"
int main(int argc, char** argv){
  cocoabot mycocoabot;
  //if no parameter was passed
  if (argc==1){
      mycocoabot.running=1;
      mycocoabot.run();

  }
  else{
      mycocoabot.run(argc, argv);
  }
 
  return 0;
}


