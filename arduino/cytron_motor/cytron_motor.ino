#include <math.h>
#define floorf floor
#define fabsf fabs

const int RIGHT_PWMpin = 8;
const int RIGHT_DIRpin = 9;
const int RIGHT_ENCAint = 0;// is actually pin 2
const int LEFT_PWMpin = 10;
const int LEFT_DIRpin = 11;
const int LEFT_ENCAint = 1; // is actually pin 3

volatile int RIGHT_rots = 0; // must be made voltaile to be updated in updateRightRots
volatile int LEFT_rots = 0; // must be made volatile to be updated in updateLeftRots

const double wheel_circ = 12.174; // inches of the wheel
const double robot_circ = 28.274; // inches of the circle the wheels rotate around

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

void updateRightRots() {
  if (digitalRead(RIGHT_DIRpin)) { // should be 1 if speed < 0
    RIGHT_rots = RIGHT_rots - 1; // backwards
  }
  else {
    RIGHT_rots = RIGHT_rots + 1; // forwards
  }
}

void rotateDeg(double deg, double speed) {
  int dir;
  if (deg >= 0) {
    dir = -1; // left
  }
  else {
    dir = 1; // right
  }
  deg = fabs(deg);
  double dist_needed = (deg/360.0)*robot_circ;
  int rots_needed = (int) round(32*30.0*dist_needed/wheel_circ); // 30 because of gear ratio
  Serial.println((int)dist_needed);
  Serial.println(rots_needed);
  Serial.println(dir);
  rotateWheels(rots_needed, dir, speed);  
}

void rotateWheels(int rots_needed, int dir, double speed) {
  int current_lrots = LEFT_rots;
  int current_rrots = RIGHT_rots;
  speed = fabs(speed);
  
  setMotorSpeed(LEFT_PWMpin,LEFT_DIRpin,float(dir)*speed);
  setMotorSpeed(RIGHT_PWMpin,RIGHT_DIRpin,float(dir)*speed);
  while ((abs(LEFT_rots - current_lrots) <= rots_needed) || (abs(RIGHT_rots - current_rrots) <= rots_needed)) {
    // do nothing
  }
  setMotorSpeed(LEFT_PWMpin,LEFT_DIRpin,0.0);
  setMotorSpeed(RIGHT_PWMpin,RIGHT_DIRpin,0.0);
}

void setup() {
  Serial.begin(9600);
  pinMode(RIGHT_DIRpin,OUTPUT);
  pinMode(LEFT_DIRpin,OUTPUT);
  attachInterrupt(RIGHT_ENCAint, updateRightRots, CHANGE);
  attachInterrupt(LEFT_ENCAint, updateLeftRots, CHANGE);
  analogWrite(RIGHT_PWMpin,0.0);
  digitalWrite(RIGHT_DIRpin,0);
  analogWrite(LEFT_PWMpin,65y0.0);
  digitalWrite(LEFT_DIRpin,0);
}

void loop() {
  double speed = 0.1;
  // aim is to rotate the robot 45* left, then 45* right.
  while (running) {
    Serial.println("90");
    rotateDeg(90.0,speed);
    delay(2000);
    Serial.println("-90");
    rotateDeg(-90.0,speed);
    delay(2000);
  }
}
