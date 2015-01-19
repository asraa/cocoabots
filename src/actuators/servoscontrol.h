#ifndef SERVOSCONTROL_H
#define SERVOSCONTROL_H

#include "../configFile.h"
#include <thread>

class servosControl
{
public:
    servosControl();
    ~servosControl();

    double armAngle;
    double hookAngle;
    double sortAngle;
    int running;

    std::thread *runThread;
    static void run(servosControl * myservos);
    //void computeNewServoAngles();

    void hookBlock();
    void raiseBlock();
    void sortRed();
    void sortGreen();
};

#endif // SERVOSCONTROL_H
