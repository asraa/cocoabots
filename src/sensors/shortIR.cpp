#include "shortIR.h"
#include <csignal>
#include <iostream>
#include <sys/time.h>
#include "mraa.hpp"

shortIR::shortIR(int dataPin){
	data_gpio = new mraa::Gpio(dataPin);
	if (data_gpio == NULL){
		return;
	}

	data_gpio->dir(mraa::DIR_IN);

	//mraa_result_t data_response = data_gpio->dir(mraa::DIR_IN);
	//if (data_response != MRAA_SUCCESS){
	//	mraa::printError(data_response);
	//	return 1;
	//}

	//replace with a check if working

	myDataPin = dataPin;
}

long shortIR::timing(){
	return data_gpio->read();
}

long shortIR::ranging(int sys){
	return 0;
}


