#include "Ultrasonic.h"
#include <LiquidCrystal.h>

Ultrasonic ultrasonic(12,13);

void setup() {
delay(1000);
Serial.begin(9600);
}

void loop()
{
  //lcd.clear();
  Serial.println(ultrasonic.Ranging(CM));
    
  delay(100);
}




