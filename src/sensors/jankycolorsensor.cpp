#include "jankycolorsensor.h"

jankyColorSensor::jankyColorSensor(int colorSensorPin){

    colorSensorAio = new mraa::Aio(colorSensorPin);
    if (colorSensorAio == NULL){
        return;
    }
    int myDataPin = colorSensorPin;
}


double jankyColorSensor::getData() {
    return colorSensorAio->read();
}
