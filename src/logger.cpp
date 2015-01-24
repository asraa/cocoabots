#include "logger.h"
#include <unistd.h>
#include <fstream>
#include "configFile.h"
int logger::myLogRate = 0;
motorsControl* logger::myMotorsPt=NULL;
servosControl* logger::myServosPt =NULL;
sensorsModule* logger::mySensorsPt =NULL;
ImageProcessor* logger::myImageProcessorPt=NULL;
int logger::singleton=0;
states** logger::myStateDblPt =NULL;
std::ofstream logger::logfile("log.txt",std::ofstream::out |  std::ios_base::app);



logger::logger(sensorsModule *sensorsPtr,
         motorsControl *motorsPtr,
         servosControl *servosPtr,
         states **statesDblPtr,
         ImageProcessor *ImageProcessorPtr,
         std::string logFileName,
         int logRateMs)
{
#if LOGGING
    if(!singleton){
        mySensorsPt = sensorsPtr;
        myMotorsPt = motorsPtr;
        myServosPt = servosPtr;
        myStateDblPt = statesDblPtr;
        myImageProcessorPt = ImageProcessorPtr;
        singleton=1;
        myLogRate=logRateMs;
        if(logfile.is_open())
            logfile.close();
        logfile.open(logFileName, std::ios_base::app);
        if(myLogRate)
            runThread = new std::thread(run,this);
    }
    else{
    }
#endif
}


void logger::log() {
#if LOGGING
    if (mySensorsPt){
  logfile << "my Sensors:" <<

             "\n Time in Microseconds"<< mySensorsPt->timeMicrosecondsSinceEpoch <<

             "\n Short IRs:" <<
             "  back:" <<mySensorsPt->backShortIRData<<
             "  front:"<<mySensorsPt->frontShortIRData <<
             "  left:"<<mySensorsPt->leftShortIRData <<
             "  right:"<<mySensorsPt->rightShortIRData <<

             "\n UltraShort IRs:" <<
             "  back:" <<mySensorsPt->backUltraShortIRData<<
             "  front:"<<mySensorsPt->frontUltraShortIRData <<
             "  left:"<<mySensorsPt->leftUltraShortIRData <<
             "  right:"<<mySensorsPt->rightUltraShortIRData <<

             "\n Ultrasonics:" <<
             "  front:"<<mySensorsPt->frontUltrasonicData <<
             "  left:"<<mySensorsPt->leftUltrasonicData <<
             "  right:"<<mySensorsPt->rightUltrasonicData <<

             "\n Encoders:" <<
             "  left:"<<mySensorsPt->leftEncoderMovement <<
             "  right:"<<mySensorsPt->rightEncoderMovement <<
             "  angle:"<<mySensorsPt->encoderAngle <<

             "\n Gyroscope:" <<
             "  Gyroscope Angle:"<<mySensorsPt->gyroscopeAngle <<
             "  Gyroscope Reading:"<<mySensorsPt->gyroscopeReading << std::endl;
    }
    if (myMotorsPt){
  logfile << "my MotorControl:" <<
             "\n Time:" <<
             "  previousTime:"<<myMotorsPt->previousTime <<

             "\n Speed:" <<
             "  realSpeed:" <<myMotorsPt->realSpeed <<
             "  realAngularSpeed:"<<myMotorsPt->realAngularSpeed <<
             "  normalizedLeftWheelSpeed:"<< myMotorsPt->normalizedLeftWheelSpeed <<
             "  normalizedRightWheelSpeed:"<<myMotorsPt->normalizedRightWheelSpeed <<

             "\n Powers:" <<
             "  left:"<<myMotorsPt->leftMotorPower <<
             "  right:"<<myMotorsPt->rightMotorPower <<

             "\n Angle:" <<
             "  previousAngle:"<<myMotorsPt->previousAngle <<

             "\n Position:" <<
             "  previousPosition:"<<myMotorsPt->previousPosition <<
             "  previousLeftWheelPosition:"<<myMotorsPt->previousRightWheelPosition <<
             "  previousRightWheelPosition:"<<myMotorsPt->previousLeftWheelPosition <<

             "\n Desired:" <<
             "  Desired position:"<<myMotorsPt->desiredPosition <<
             "  Desired Angle:"<<myMotorsPt->desiredAngle <<std::endl;
    }
    if (myServosPt){
  logfile << "my ServoControl:" <<

             "\n Angle:" <<
             "  armAngle:" <<myServosPt->armAngle <<
             "  hookAngle:"<<myServosPt->hookAngle <<
             "  sortAngle:"<< myServosPt->sortAngle <<

             "\n sweeping:" <<
             "  swipping:"<<myServosPt->swipping <<
             " previousSwipe:"<<myServosPt->previousSwipe <<std::endl;
    }
    if (myStateDblPt && *myStateDblPt){
  logfile << "my StateMachine:" <<

             "\n Name:" <<
             "  name:" << (*myStateDblPt)->getName() <<
             "  nextStatename:" << (*myStateDblPt)->getNextState()->getName() <<


             "\n Times:" <<
             "  Running Time:" << (*myStateDblPt)->getRunningTimeMicroSeconds() <<
             "  Total Time:" << (*myStateDblPt)->getTimeMicroseconds() <<
             "  Remaining Time:" << (*myStateDblPt)->getTimeRemainingGameSeconds() <<

             "\n States Variables:" <<
             "  Wall Followed:" << (*myStateDblPt)->wallFollowed <<
             "  Wall Following:" << (*myStateDblPt)->wallFollowing<<

             "  \nColected Blocks:" << (*myStateDblPt)->collectedBlocks <<
             "  Colecting Blocks:" << (*myStateDblPt)->collectingBlocks <<
             "  Finished Colecting Blocks:" << (*myStateDblPt)->collectingBlocks <<

             "  \nWent to Point:" << (*myStateDblPt)->wentToPoint <<
             "  Going to Point:" << (*myStateDblPt)->goingToPoint <<
             "  Finished going to Point:" << (*myStateDblPt)->finishedGoingToPoint <<

             "\n Computer Vision:" <<
             "   Found Cube:" << (*myStateDblPt)->foundCube() <<

             "\n Distances:" <<
             "   Distance To carrot:" << (*myStateDblPt)->getDistanceToCarrot() <<
             "   Distance To Walls:" <<
             "   front:" << (*myStateDblPt)->getDistanceFrontWall() <<
             "   right:" << (*myStateDblPt)->getDistanceRightWall() <<std::endl;
    }
    if (myImageProcessorPt){
  logfile << "my ImageProcessor:" <<

             "\n Cube:" <<
             "  found:" <<myImageProcessorPt->getFoundCube() <<
             "  Angle:" <<myImageProcessorPt->getNearestCubeAngle() <<
             "  Distance:" <<myImageProcessorPt->getNearestCubeDist() <<
             "  Color:" <<myImageProcessorPt->getNearestCubeColor() <<std::endl;
    myImageProcessorPt->writeToFile("cameraPicture.jpg");
    }

#endif
}

void logger::log(std::string message) {
#if LOGGING
    logfile << message << std::endl;
    log();
#endif
}


void logger::run(void * loggerPtr){
    logger * myLogger = (logger *)loggerPtr;
    while(myLogger->running){
        logger::log("Automatic log");
        usleep(logger::myLogRate*1000);
    }
}

logger::~logger(){
    running=0;
    if(runThread->joinable()){
        runThread->join();
        delete runThread;
    }
}
