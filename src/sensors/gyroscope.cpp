#include "gyroscope.h"

gyroscope::gyroscope(int chipSelectPin, int spiPin): 
chipSelectGpio(chipSelectPin), spiSpi(spiPin){
	running = 1;

	chipSelectGpio.dir(mraa::DIR_OUT);
	chipSelectGpio.write(1);

	spiSpi.bitPerWord(32);
	
	sensorRead = 0x20000000;
	writeBuf[0] = sensorRead & 0xff;
  	writeBuf[1] = (sensorRead >> 8) & 0xff;
  	writeBuf[2] = (sensorRead >> 16) & 0xff;
  	writeBuf[3] = (sensorRead >> 24) & 0xff;
  	total = 0;
  	init = 0;

	runThread = new std::thread(run, this);
}

gyroscope::~gyroscope() {
	running = 0;
	runThread->join();
	delete runThread;
}

void gyroscope::run(void* gyroscopeSensorPointer){
	gyroscope* gyroscopeSensor = (gyroscope*) gyroscopeSensorPointer;
	mraa::Gpio* chipSelect = & (gyroscopeSensor->chipSelectGpio);
	mraa::Spi* spi = & (gyroscopeSensor->spiSpi);

	while (gyroscopeSensor->running){
		chipSelect->write(0);
		uint8_t* recv = (uint8_t*) spi->write((char*)(gyroscopeSensor->writeBuf), 4);
		chipSelect->write(1);
		if (recv != NULL){
			unsigned int recvVal = ((uint8_t) recv[3] & 0xFF);
			recvVal = (recvVal << 8) | ((uint8_t)recv[2] & 0xFF);
      		recvVal = (recvVal << 8) | ((uint8_t)recv[1] & 0xFF);
      		recvVal = (recvVal << 8) | ((uint8_t)recv[0] & 0xFF);
      		(gyroscopeSensor->reading) = (recvVal >> 10) & 0xffff;
      		if (gyroscopeSensor->init) {
      			unsigned long long ms = (unsigned long long)(gyroscopeSensor->tv.tv_sec)*1000 +
      			(unsigned long long)(gyroscopeSensor->tv.tv_usec) / 1000;
				gettimeofday(&gyroscopeSensor->tv, NULL);
				ms -= (unsigned long long)(gyroscopeSensor->tv.tv_sec)*1000 +
				(unsigned long long)(gyroscopeSensor->tv.tv_usec) / 1000;
				int msi = (int)ms;
				gyroscopeSensor->msf = (float)msi;
				float rf = (float)(gyroscopeSensor->reading);
    		    gyroscopeSensor->total += -0.001 * 
(gyroscopeSensor->msf) * (rf / 80.0);
    		}
     		else {
     			gyroscopeSensor->init = 1;
				gettimeofday(&gyroscopeSensor->tv, NULL);
    		}
    	}
    	usleep(10 * MS);
    }
}

float gyroscope::getTotal(){
	return (float)total;
}

float gyroscope::getReading(){
	return (float)reading;
}

float gyroscope::getTime(){
	return (float)(-msf);
}
