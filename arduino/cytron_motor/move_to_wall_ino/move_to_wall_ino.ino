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

int readings[numReadings];
int index = 0;
int total = 0;
int average = 0;

int running = 1;

void setMotorSpeed(int pwmpin, int dirpin, double speed) {
  assert(-1.0 <= speed && speed <= 1.0);
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
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(DIRpin,OUTPUT);
  analogWrite(PWMpin,0.0);
  digitalWrite(DIRpin,0);
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  readings[index] = pulseIn(echoPin, HIGH);        // Read in times pulse
  total = total + readings[index];
  index = index + 1;
  if (index >= numReadings) {
     index = 0;
     average = total / numReadings;
     int distance = (average / 74) / 2;
                       // Calculate distance from time of pulse
     Serial.println(distance); 
     total = 0;    }    
     
  deltaDist = abs(distance) - 5;
  
  if (distance > 2 || distance > 80){
    deltaDist = 0; //don't move for weird cases
  }
  
  if (distance >= 5) {
      dir = 1; 
  }
  else {
    dir = -1
  }
  
  
  speedProp = map(deltaDist, 2, 50, -1, 1);
  
  setMotorSpeed(PWMpinL, DIRpinL, convertSpeed(speedProp*dir));
  setMotorSpeed(PWMpinR, DIRpinR, convertSpeed(speedProp*dir));
     
  delay(10000);                                    // Wait 50mS before next ranging
}

