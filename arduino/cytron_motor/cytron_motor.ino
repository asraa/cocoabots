#include <math.h>

const int PWMpin = 3;
const int DIRpin = 2;

int running = 1;

void setMotorSpeed(int pwmpin, int dirpin, double speed) {
  if (speed < 0) {
    digitalWrite(dirpin,1);
  }
  else {
    digitalWrite(dirpin,0);
  }
  analogWrite(pwmpin,convertSpeed(speed));
}

int convertSpeed(double speedToConvert) {
  speedToConvert = 255.0*speedToConvert;
  int trueSpeed = (int) floorf(fabs(speedToConvert));
  return trueSpeed;
}

void setup() {
  Serial.begin(9600);
  pinMode(DIRpin,OUTPUT);
  analogWrite(PWMpin,0);
  digitalWrite(DIRpin,0);
}

void loop() {
  double speed = -1.0;
  while (running) {
    Serial.println(String("Speed: "+String(speed)));
    Serial.println(String("AnalogWrite: "+String(convertSpeed(speed))));
    setMotorSpeed(PWMpin,DIRpin,speed);
    speed = speed + 0.1;
    if (speed > 1.0) {
      speed = -1.0;
      setMotorSpeed(PWMpin,DIRpin,0.0);
      delay(2000);
    }
    delay(500);
    
  }
}
