#include "jankycolorsensor.h"

jankyColorSensor::jankyColorSensor(int colorSensorPin){

    colorSensorAio = new mraa::Aio(colorSensorPin);
    if (colorSensorAio == NULL){
        return;
    }
    int myDataPin = colorSensorPin;
    int count = 11;
}

int jankyColorSensor::runningTotal(){
    double value = colorSensorAio->read();
    if (value > RED_THRESHOLD){
        count = 0;
    }
    else {
        count += 1;
    }
    return count;
}

double jankyColorSensor::getData() {
    return (runningTotal() < 10); // 1 if red cube in past 10 readings
}
