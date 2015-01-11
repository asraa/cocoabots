#ifndef GYROSCOPE_H
#define GYROSCOPE_H


class gyroscope
{
  public:
    ///initializes the Gyroscope
    gyroscope();
    
    ///Returns the data
    int getData();

    private:
    ///Setup the pins, etc
    void setup();
    
    ///Starts running the gyroscope
    void run();
    
};


#endif
