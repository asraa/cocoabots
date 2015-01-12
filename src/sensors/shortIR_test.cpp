#include "shortIR.h"
#include <csignal>
#include <iostream>
#include <sys/time.h>
#include "mraa.hpp"

<<<<<<< HEAD
int run_shortIR(){
=======
shortIR::shortIR(int dataPin){
	mraa::Gpio* data_gpio = new mraa::Gpio(dataPin);
	data_gpio->(mraa::DIR_IN);
	if (data_gpio == NULL){
		return MRAA_ERROR_UNSPECIFIED;
	}
	
	mraa_result_t data_response = data_gpio->dir(mraa::DIR_IN);
	if (data_response != MRAA_SUCCESS){
		mraa::printError(data_response);
		return 1;
	}
	data_pin = dataPin;
}

long shortIR::timing(){
	return data_gpio->read();
}

long shortIR::ranging(int sys){
	return 0;
}

int runShortIR(){
>>>>>>> 1d80377e48f15f93c0b5239beea6d8224432affb
	usleep(50000.0);
	shortIR aShortIR(2);
	reading = aShortIR.timing();
	std::cout << reading << std::endl;
}


