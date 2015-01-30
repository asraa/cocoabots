#include "jankycolorsensor.h"

jankyColorSensor::jankyColorSensor(int colorSensorPin){

    data_gpio = new mraa::Gpio(colorSensorPin);
    if (data_gpio == NULL){
        return;
    }

    data_gpio->dir(mraa::DIR_IN);

    myDataPin = colorSensorPin;
}



double jankyColorSensor::getData() {
    return data_gpio->read();
}
