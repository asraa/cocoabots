#include "encoder.h"

encoder::encoder(int dirPin, int encPin): dirGpio(dirPin), 
encGpio(encPin) {
	edgeCount = 0;
	rotations = 0.0;
	running = 1;

//	dirGpio.dir(mraa::DIR_OUT);
	encGpio.dir(mraa::DIR_OUT);
	encGpio.isr(mraa::EDGE_BOTH, edge_handler, this);

    //runThread = new std::thread(run,this);
}

encoder::~encoder() {
	running = 0;
    //runThread->join();
    //delete runThread;
}

void encoder::edge_handler(void* encoderSensorPointer) {
	encoder* encSensor = (encoder*) encoderSensorPointer;
	mraa::Gpio* dir = & (encSensor->dirGpio);

	int offset;
	// if dir is 1, then wheels are rotating backwards
	if (dir->read() == 1) {
		offset = -1;
	}
	// if dir is 0, then wheels are rotating forwards
	else {
		offset = 1;
	}

	encSensor->edgeCount = encSensor->edgeCount + offset;
}

void encoder::run(void* encoderSensorPointer) {
	encoder* encSensor = (encoder*) encoderSensorPointer;
	while (encSensor->running) {
		// do nothing lol
		// we're just waiting for interrupts on the encoder pin
	}
}

long long encoder::getCounts() {
	return edgeCount;
}

double encoder::getRotations() {
	rotations = (edgeCount / EDGES_PER_ROTATION) / GEAR_RATIO;
	return rotations;
}

double encoder::getData(){
    return getRotations();
}
