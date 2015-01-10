#include <math.h>
#define floorf floor

const int RIGHT_PWMint = 0; // is actually pin 2
const int RIGHT_DIRpin = 9;
const int RIGHT_ENCApin = 7;
const int LEFT_PWMpin = 10;
const int LEFT_DIRpin = 11;
const int LEFT_ENCAint = 1; // is actually pin 3

volatile int RIGHT_rots = 0; // must be made voltaile to be updated in updateRightRots
volatile int LEFT_rots = 0; // must be made volatile to be updated in updateLeftRots

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

void updateLeftRots() {
  if (digitalRead(LEFT_DIRpin)) { // should be 1 if speed < 0
    LEFT_rots = LEFT_rots - 1; // backwards
  }
  else {
    LEFT_rots = LEFT_rots + 1; // forwards
  }
}

void updateLeftRots() {
  if (digitalRead(RIGHT_DIRpin)) { // should be 1 if speed < 0
    RIGHT_rots = RIGHT_rots - 1; // backwards
  }
  else {
    RIGHT_rots = RIGHT_rots + 1; // forwards
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(RIGHT_DIRpin,OUTPUT);
  pinMode(LEFT_DIRpin,OUTPUT);
  attachInterrupt(RIGHT_ENCAint, updateRightRots, CHANGE);
  attachInterrupt(LEFT_ENCAint, updateLeftRots, CHANGE);
  analogWrite(LEFT_PWMpin,0);
  digitalWrite(LEFT_DIRpin,0);
}

void loop() {
  double speed = 0.1;
  // aim is to rotate the wheels half a turn, then stop.
  while (LEFT_rots < 30*16) { // is 30*16 because 30:1 gear, and 16/32 = half turn. (32 changes of voltage per rotation)
    Serial.println(String(LEFT_rots));
    setMotorSpeed(LEFT_PWMpin,LEFT_DIRpin,speed);
  }
  setMotorSpeed(LEFT_PWMpin,LEFT_DIRpin,0.0);
  Serial.println(LEFT_rots);
  Serial.println("Done!");
  delay(2000);
  LEFT_rots = 0;
}
