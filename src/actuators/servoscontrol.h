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

    volatile int swipping;
    int previousSwipe;

    std::thread *runThread;
    static void run(servosControl * myservos);

    servosControl();
    ~servosControl();
    void computeNewServosAngles();
    void hookBlock();
    void unHookBlock();
    void raiseBlock();
    void sortRight();
    void sortLeft();
    void reset();
    void sweep();
    void stopSweep();
};

#endif // SERVOSCONTROL_H
