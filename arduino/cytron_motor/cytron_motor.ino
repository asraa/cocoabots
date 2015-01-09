#define __ASSERT_USE_STDERR
#include <assert.h>
#include <math.h>

const int PWMpin = 2;
const int DIRpin = 3;

int running = 1;

void setMotorSpeed(int pwmpin, int dirpin, double speed) {
  assert(-1.0 <= speed && speed <= 1.0);
  if (speed < 0) {
    digitalWrite(dirpin,1);
  }
  else {
    digitalWrite(dirpin,0);
  }
  analogWrite(pwmpin,fabs(speed));
}
  
void setup() {
  Serial.begin(9600);
  pinMode(DIRpin,OUTPUT);
  analogWrite(PWMpin,0.0);
  digitalWrite(DIRpin,0);
}

void loop() {
  double speed = -1.0;
  while (running) {
    Serial.println(String("Speed: "+String(speed)));
    setMotorSpeed(PWMpin,DIRpin,speed);
    speed = speed + 0.1;
    if (speed > 1.0) {
      speed = -1.0;
      setMotorSpeed(PWMpin,DIRpin,0.0);
      delay(2000);
    }
    delayMicroseconds(100000);
  }
}
