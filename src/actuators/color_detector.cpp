#include "mraa.hpp"
#include <cassert>
#include <csignal>
#include <iostream>
#include <cstdint>
#include "color_detector.h"

int running = 1;

//uses A3 pin for voltage reading from photoresistor and PWM 4 for LED

color_detector::color_detector(int redPWMind, int greenPWMind, int dataPin):redPWM(redPWMind), greenPWM(greenPWMind)
{
    data_aio = new mraa::Aio(dataPin);
    if (data_aio == NULL){
        return;
    }

    int myDataPin = dataPin;

}
