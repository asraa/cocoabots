#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

class instructions
{
public:
    instructions();
    int raiseArm;
    int sweep;
    int grab;
    int updated;
    float distance;
    float angle;
    float servoAngle1;
    float servoAngle2;
    int wallFollow;
};

#endif // INSTRUCTIONS_H
