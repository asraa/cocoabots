#ifndef PID_H
#define PID_H
#define epsilon 0.01
#define dt 0.01

#include <sys/time.h>

class pid {
       public:
            double kp, ki, kd;
            float calcPID(float desired, float actual);
       private:

};


#endif // PID_H
