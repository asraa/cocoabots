#include "ultraShortIR.h"
#include <csignal>
#include <iostream>
#include <cmath>
#include <sys/time.h>
#include "mraa.hpp"
#include <unistd.h>


ultraShortIR::ultraShortIR(int dataPin){
  	data_gpio = new mraa::Gpio(dataPin);
	if (data_gpio == NULL){
		return;
	}

	data_gpio->dir(mraa::DIR_IN);

	int myDataPin = dataPin;
}

int ultraShortIR::getData(){
	return data_gpio->read();
}
