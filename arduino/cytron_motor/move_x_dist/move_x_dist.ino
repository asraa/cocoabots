#include <math.h>
#define floorf floor

const int RIGHT_PWMpin = 8; // is actually pin 2
const int RIGHT_DIRpin = 9;
const int RIGHT_ENCAint = 0;
const int LEFT_PWMpin = 10;
const int LEFT_DIRpin = 11;
const int LEFT_ENCAint = 1; // is actually pin 3

volatile int RIGHT_rots = 0;
volatile int LEFT_rots = 0;

int targetDist = 15; //unit inches

long radius = 3.825;
int running = 1;
long revs = 0;
long circumference = 3.14*radius;
long targetRevs = targetDist / circumference; 
int gearRatio = 30;



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

void updateLeftRots() {
  if (digitalRead(LEFT_DIRpin)) { // should be 1 if speed < 0
    LEFT_rots = LEFT_rots - 1; // backwards
  }
  else {
    LEFT_rots = LEFT_rots + 1; // forwards
  }
}

void updateRightRots() {
  if (digitalRead(RIGHT_DIRpin)) { // should be 1 if speed < 0
    RIGHT_rots = RIGHT_rots - 1; // backwards
  }
  else {
    RIGHT_rots = RIGHT_rots + 1; // forwards
  }
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(RIGHT_ENCAint, updateRightRots, CHANGE);
  attachInterrupt(LEFT_ENCAint, updateLeftRots, CHANGE);
  pinMode(LEFT_PWMpin,OUTPUT);
  pinMode(RIGHT_PWMpin, OUTPUT);
  analogWrite(LEFT_PWMpin,0);
  digitalWrite(LEFT_DIRpin,0);
  analogWrite(RIGHT_PWMpin, 0);
  digitalWrite(RIGHT_DIRpin, 0);
}

void loop() {
  double speed = .1;
  while (LEFT_rots < gearRatio*targetRevs) {
    setMotorSpeed(LEFT_PWMpin,LEFT_DIRpin,speed); 
  }
  while (RIGHT_rots < gearRatio*targetRevs) {
    setMotorSpeed(RIGHT_PWMpin, RIGHT_DIRpin,speed);
  }

    delay(500);
    
  }

