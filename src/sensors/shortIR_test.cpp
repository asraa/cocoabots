#include "shortIR.h"
#include <csignal>
#include <iostream>
#include <sys/time.h>
#include "mraa.hpp"

int run_shortIR(){
	usleep(50000.0);
	shortIR aShortIR(2);
	reading = aShortIR.timing();
	std::cout << reading << std::endl;
}


