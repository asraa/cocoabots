#ifndef SERVOSCONTROL_H
#define SERVOSCONTROL_H

#include "../configFile.h"
#include <thread>

class servosControl
{
public:
    double armAngle;
    double hookAngle;
    double sortAngle;
    int running;
    int startCollect;
    int timeStart;

    int swipping;
    int previousSwipe;

    std::thread *runThread;
    static void run(servosControl * myservos);

    servosControl();
    ~servosControl();
    void computeNewServosAngles();
    void hookBlock();
    void unHookBlock();
    void raiseBlock();
    void sortRed();
    void sortGreen();
    void reset();
    void sweep();
    void stopSweep();
};

#endif // SERVOSCONTROL_H
