#ifndef SERVOSCONTROL_H
#define SERVOSCONTROL_H

#include "../configFile.h"

class servosControl
{
public:
    servosControl();

    double armAngle;
    double hookAngle;
    double sortAngle;

    void hookBlock();
    void raiseBlock();
    void sortRed();
    void sortGreen();
};

#endif // SERVOSCONTROL_H
