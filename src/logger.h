/*  This is a simple logger to help us debug the robot.
 *  It is not in anyway thread safe. If two threads try to write to the log at the same time,
 *  both messages will be written to the log merged. They would look like gibberish.
 *
 * This is not intentional. but will not be fixed because the probability of collision for the intended use
 *  is smaller than 1/100, so only ~1 in every 100 messages would get merged together,
 * and most would be fine.
 *
 */
#ifndef LOGGER_H
#define LOGGER_H
#include <thread>
#include <string>

#include "sensorsmodule.h"
#include "actuators/motorscontrol.h"
#include "actuators/servoscontrol.h"
#include "imageProcessing/ImageProcessor.h"
//#include "localization/particlefilter.h"
#include "states.h"
class logger
{
public:
    logger(sensorsModule * sensorsPtr,
        motorsControl * motorsPtr,
        servosControl * servosPtr,
        states ** statesDblPtr,
        ImageProcessor * ImageProcessorPtr,
//       particleFilter * particleFilterPtr =NULL,
        std::string logFileName = LOG_FILE_NAME,
           int logRateMs=UPDATE_RATE_LOGGER);
    ~logger();

    std::thread *runThread;
    static void run(void * loggerPtr);
    static void log();
    static void log(std::string message);

    static std::ofstream logfile;
    static sensorsModule * mySensorsPt;
    static motorsControl * myMotorsPt;
    static servosControl * myServosPt;
    static states ** myStateDblPt;
    static ImageProcessor * myImageProcessorPt;
    static int myLogRate; //if 0 it doesn't log
    int running;

private:
    static int singleton;
};

#endif // LOGGER_H
