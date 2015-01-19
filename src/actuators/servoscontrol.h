#ifndef SERVOSCONTROL_H
#define SERVOSCONTROL_H

#include "../configFile.h"
#include <thread>

class servosControl
{
public:
    servosControl();
    ~servosControl();

#include  <thread>
class servosControl
{
public:
    double armAngle;
    double hookAngle;
    double sortAngle;
    int running;

    std::thread *runThread;
    static void run(servosControl * myservos);
    //void computeNewServoAngles();

    std::thread * runThread;
    servosControl();
    ~servosControl();
    static void run(servosControl *myservo);
    //void computeNewServosAngles();
    void hookBlock();
    void raiseBlock();
    void sortRed();
    void sortGreen();
};

#endif // SERVOSCONTROL_H
