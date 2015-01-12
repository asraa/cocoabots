#include "shortIR.h"
#include <csignal>
#include <iostream>
#include <sys/time.h>
#include "mraa.hpp"

shortIR::shortIR(int dataPin){
    data_gpio = new mraa::Gpio(dataPin);
    if (data_gpio == NULL){
        return;// NULL;//MRAA_ERROR_UNSPECIFIED;
    }
    data_gpio->dir(mraa::DIR_IN);
    mraa_result_t data_response = data_gpio->dir(mraa::DIR_IN);
	if (data_response != MRAA_SUCCESS){
		mraa::printError(data_response);
	}
    myDataPin = dataPin;
}

long shortIR::timing(){
    return data_gpio->read();
}

long shortIR::ranging(int sys){
	return 0;
}

int runShortIR(){
	usleep(50000.0);
	shortIR shortIRTest(2);
    int reading;
	reading = shortIRTest.timing();
	std::cout << reading << std::endl;
}


