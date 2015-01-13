#ifndef ULTRASHORTIR_H
#define ULTRASHORTIR_H


class ultraShortIR
{
  public:
    	mraa:Gpio * data_gpio;
	ultraShortIR(int dataPin);
    	int present();

    private:
    	int dataPin;

    
};


#endif
