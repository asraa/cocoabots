#ifndef LOGGER_H
#define LOGGER_H
#include <thread>
#include <string>

#include "sensorsmodule.h"
#include "actuators/motorscontrol.h"
#include "actuators/servoscontrol.h"
#include "states.h"
class logger
{
public:
    logger(sensorsModule * sensorsPtr,
        motorsControl * motorsPtr,
        servosControl * servosPtr,
        states ** statesDblPtr,
        int logRateMs, std::string logFileName);
    ~logger();
    static void log();
    static void log(std::string message);

    static std::ofstream logfile;
    static sensorsModule * mySensorsPt;
    static motorsControl * myMotorsPt;
    static servosControl * myServosPt;
    static states ** myStateDblPt;
    static int myLogRate; //if 0 it doesn't log


private:
    static int singleton;
};

#endif // LOGGER_H
