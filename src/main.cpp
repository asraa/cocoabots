#include "cocoabot.h"



int main(int argc, char** argv){
  
  //if no parameter was passed
  if (argc==1){
     cocoabot mycocoabot(); //returns after the end of the round
  }
  else{
      //custom execution.
      cocoabot mycocoabot(argc, argv); //returns after the end of the test.
  }
 
  return 0;
}


