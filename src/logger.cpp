#include "logger.h"
#include <fstream>

int logger::myLogRate = 0;
motorsControl* logger::myMotorsPt=NULL;
servosControl* logger::myServosPt =NULL;
sensorsModule* logger::mySensorsPt =NULL;
int logger::singleton=0;
states** logger::myStateDblPt =NULL;
std::ofstream logger::logfile("log.txt", std::ios_base::app);



logger::logger(sensorsModule *sensorsPtr,
         motorsControl *motorsPtr,
         servosControl *servosPtr,
         states **statesDblPtr,
         int logRateMs,
               std::string logFileName)
{
    if(!singleton){
        mySensorsPt = sensorsPtr;
        myMotorsPt = motorsPtr;
        myServosPt = servosPtr;
        myStateDblPt = statesDblPtr;
        singleton=1;
        logfile.open(logFileName, std::ios_base::app);

        if(logRateMs){ //Run Thread automatically

        }
    }
    else{
    }

}


void logger::log() {

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

  logfile << "my ServoControl:" <<

             "\n Angle:" <<
             "  armAngle:" <<myServosPt->armAngle <<
             "  hookAngle:"<<myServosPt->hookAngle <<
             "  sortAngle:"<< myServosPt->sortAngle <<

             "\n sweeping:" <<
             "  swipping:"<<myServosPt->swipping <<
             " previousSwipe:"<<myServosPt->previousSwipe <<std::endl;

  logfile << "my StateMachine:" <<

             "\n Name:" <<
             "  name:" << (*myStateDblPt)->getName() <<

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

void logger::log(std::string message) {
    logfile << message << std::endl;
    log();
}


logger::~logger(){

}
