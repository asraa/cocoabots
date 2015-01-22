#ifndef COLOR_DETECTOR_H
#define COLOR_DETECTOR_H
#include "mraa.hpp"
#include <stdlib.h>
#include <cassert>
#include <csignal>
#include <iostream>
#include "../configFile.h"

class color_detector
{
public:
    color_detector(int redPWMind, int greenPWMind, int dataPin);
    int redPWM;
    int greenPWM;
    mraa::Aio * data_aio;

private:
    int myDataPin;
};


#endif // COLOR_DETECTOR_H
