#ifndef ULTRASHORTIR_H
#define ULTRASHORTIR_H
#include "mraa.hpp"


class ultraShortIR
{
  public:
    	mraa::Gpio * data_gpio;
	ultraShortIR(int dataPin);
    	int getData();

    private:
    	int myDataPin;

    
};


#endif
