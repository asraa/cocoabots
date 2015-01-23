#include "shortIR.h"
#include <csignal>
#include <iostream>
#include <cmath>
#include <sys/time.h>
#include "mraa.hpp"
#include <unistd.h>


shortIR::shortIR(int dataPin){

	data_aio = new mraa::Aio(dataPin);
	if (data_aio == NULL){
		return;
	}

	//data_aio->dir(mraa::DIR_IN);

	//replace with a check if working


	int myDataPin = dataPin;
}

float shortIR::timing(){
	return data_aio->read();
}

float shortIR::ranging(){
	float rawData = timing();
    return 12343.85 * pow(rawData,-1.15);	 //Data in centimeters
}

double shortIR::getData(){
    return ranging() * 0.3937; // Data in inches
}
