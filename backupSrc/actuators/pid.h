#ifndef PID_H
#define PID_H
#define epsilon 0.01
#define dt 0.01

#include <sys/time.h>

class pid {
       public:
            pid(float kp, float ki,float kd);
            float calcPID(float desired, float actual);
       private:
            float KP, KI, KD;

};


#endif // PID_H
