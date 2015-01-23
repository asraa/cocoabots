#include "jankycolorsensor.h"

jankyColorSensor::jankyColorSensor(int colorSensorPin): 
colorSensorAio(colorSensorPin) {
	dataInt = 0;
	dataVolt = 0.0;

	running = 1;
	
	runThread = new std::thread(run, this);
}

jankyColorSensor::~jankyColorSensor() {
	running = 0;
	runThread->join();
	delete runThread;
}

void jankyColorSensor::run(void* jankyColorSensorPointer){
	jankyColorSensor* jankyColorSensorSensor = (jankyColorSensor*) jankyColorSensorPointer;
	mraa::Aio* colorSensor = & (jankyColorSensorSensor->colorSensorAio);

	while (jankyColorSensorSensor->running){
		jankyColorSensorSensor dataInt = colorSensor.read();
    	usleep(10 * MS);
    }
}

float jankyColorSensor::getDataV() {
	dataVolt = dataInt * INT_TO_VOLTS;
	return dataVolt;
}

int jankyColorSensor::getData() {
	return dataInt;
}
