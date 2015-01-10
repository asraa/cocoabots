#define __ASSERT_USE_STDERR
#include <assert.h>
#include <math.h>

const int trigPin = 2;
const int echoPin = 3;

const int PWMpinL = 8;
const int DIRpinL = 9;
const int PWMpinR = 10;
const int DIRpinR = 11;

const int numReadings = 10;

float readings[numReadings];
int index = 0;
float total = 0;
int average = 0;
float targetDist = 5.0;

int running = 1;

void setup() {
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT); 
  
  pinMode(DIRpinL,OUTPUT);
  analogWrite(PWMpinL,0.0);
  digitalWrite(DIRpinL,0);
  pinMode(DIRpinR,OUTPUT);
  analogWrite(PWMpinR,0.0);
  digitalWrite(DIRpinR,0);
  
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
}

int getSignal(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH);
}

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

void loop() {
  running = 1;
  readings[index] = getSignal();
  //Serial.println(readings[index]);  // Read in times pulse
  total = total + readings[index];
  index = index + 1;
  if (index >= numReadings) {
     index = 0;
     average = total / numReadings;
     float distance = (average / 74) / 2;
     Serial.println(distance); 
     
     if (distance < 2 || distance > 100){
       running = 0; }
     
     float deltaDist = abs(distance) - targetDist;
     int dir = 1;
     
     if (distance < 5){dir = -1;}
     Serial.println(deltaDist);
     
     float spd = deltaDist/10.0;
     spd = int(spd);
     float speed = spd/10.0
     
     Serial.println(spd);
     if (running) {
       setMotorSpeed(PWMpinL, DIRpinL, speed*dir);
       setMotorSpeed(PWMpinR, DIRpinR, dir*speed);
       delay(500);
     }
      total = 0;
  }
  delay(100);                                    // Wait 50mS before next ranging
}

