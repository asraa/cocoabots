#include "motorwheel.h"

motorWheel::motorWheel(int dirPinN, int pwmInd):dirPin(dirPinN),pwmIndex(pwmInd)
{
    dirPin.dir(mraa::DIR_OUT);
}
