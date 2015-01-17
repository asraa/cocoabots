#ifndef CONFIGFILE_H
#define CONFIGFILE_H

//Motor definitions
#define MOTORS_OPPOSITE 1 //IF THE MOTORS ARE WIRED OPPOSITELY

//SWAP THE MOTOR_DIRECTIONS BACK AND FORTH IF THE ROBOT IS GOING BACKWARDS
// If you swap, you also need to modify the GYROSCOPE_CLOCKWISE_POSITIVE from +-1 to -+
// And also change ENCODER_OPPOSITE_MOTOR from 0 to 1;
//IF YOU MODIFY THEM, REMEMBER TO RETUNE THE MOTORCONTROL GAINS. THEY ARE SMALLER IF THE
//METAL WHELL IS ON THE FRONT.
#define MOTOR_DIRECTION_BACK 1  //Defines the value of the Dir pin when going back
#define MOTOR_DIRECTION_FRONT 0  //Defines the value of the Dir pin when going front
#define GYROSCOPE_CLOCKWISE_POSITIVE -1 //Change from +1 to -1 if it is negative in the clockwise direction
#define ENCODER_OPPOSITE_MOTOR 1 // +1 or -1 If the encoders are defined to be going opposite to the motor direction

#define RIGHT_WHEEL 1
#define RIGHT_WHEEL_PWM 1
#define RIGHT_WHEEL_DIR 9

#define LEFT_WHEEL 1
#define LEFT_WHEEL_PWM 0
#define LEFT_WHEEL_DIR 6


#define CIRCUMFERENCE_WHEEL 12.1738 // inches
#define DISTANCE_BETWEEN_WHEELS 0

//Define values for the motors
#define MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS 0.7
#define MAXIMUM_REVOLUTIONS_PER_SECOND 3 //Useful to control the torque limiter
#define MAXIMUM_SPEED MAXIMUM_REVOLUTIONS_PER_SECOND
#define MAXIMUM_DEGREES_PER_SECOND 360
#define MAXIMUM_ANGULAR_SPEED MAXIMUM_DEGREES_PER_SECOND

//Servo definitions
#define SORT_SERVO_PWM 15
#define ARM_SERVO_PWM 14
#define HOOK_SERVO_PWM 13


//SENSORS
//Ultrasonic
#define FRONT_ULTRASONIC 1
#define FRONT_ULTRASONIC_TR 0
#define FRONT_ULTRASONIC_EC 1

#define RIGHT_ULTRASONIC 0
#define RIGHT_ULTRASONIC_TR 0
#define RIGHT_ULTRASONIC_EC 0

#define LEFT_ULTRASONIC 0
#define LEFT_ULTRASONIC_TR 0
#define LEFT_ULTRASONIC_EC 0


//Encoder
#define GEAR_RATIO 30.0
#define EDGES_PER_ROTATION 32.0

#define ENC_2_WIRES 0 //If we are using two wires per encoder, and using quadrature to tell the direction
#define LEFT_ENCODER 1
#define RIGHT_ENCODER 1

#define LEFT_ENCODER_ENC_A 4 //Yellow wire or Blue
#define LEFT_ENCODER_ENC_B 5 //White wire

#define RIGHT_ENCODER_ENC_A 10 //Yellow wire or Blue
#define RIGHT_ENCODER_ENC_B 11 //White wire

#define LEFT_ENCODER_DIR LEFT_WHEEL_DIR
#define LEFT_ENCODER_ENC 4

#define RIGHT_ENCODER_DIR RIGHT_WHEEL_DIR
#define RIGHT_ENCODER_ENC 5

//UltrashortIR
#define FRONT_ULTRASHORTIR 0
#define FRONT_ULTRASHORTIR_PIN 0

#define LEFT_ULTRASHORTIR 0
#define LEFT_ULTRASHORTIR_PIN 0

#define RIGHT_ULTRASHORTIR 0
#define RIGHT_ULTRASHORTIR_PIN 0

#define BACK_ULTRASHORTIR 0
#define BACK_ULTRASHORTIR_PIN 0


//ShortIR
#define FRONT_SHORTIR 0
#define FRONT_SHORTIR_PIN 0

#define LEFT_SHORTIR 0
#define LEFT_SHORTIR_PIN 0

#define RIGHT_SHORTIR 0
#define RIGHT_SHORTIR_PIN 0

#define BACK_SHORTIR 0
#define BACK_SHORTIR_PIN 0

//Gyroscope
//  FOR CHIP_PIN =10 and SPI =0 we have:
// CS =10
// MOSI = 11
// MISO =12
// SERIAL =13
#define GYROSCOPE 1
#define GYROSCOPE_CHIP_PIN 10
#define GYROSCOPE_SPI_PIN 0



//Define values for the alpha parameter of the filter of the sensors
#define SHORT_IR_ALPHA 0.7
#define ULTRASHORT_IR_ALPHA 0.5
#define ULTRASONIC_ALPHA 0.7
#define ENCODER_ALPHA 0
#define GYROSCOPE_TOTAL_ALPHA 0.7
#define GYROSCOPE_READING_ALPHA 0.7

//Sensors update Rate
#define SENSORS_UPDATE_RATE_MILISECONDS 0 //UPDATE AS OFTEN AS POSSIBLE

//Speed control
//Define gains for the angle and position control.
#define SPEED_CONTROL_UPDATE_RATE_MILISECONDS 50
#define FWD_ERROR_GAIN (0.30/CIRCUMFERENCE_WHEEL)    //Should be positive
#define FWD_SPEED_GAIN (-0.0/CIRCUMFERENCE_WHEEL)  //Should be negative
#define ANG_ERROR_GAIN (0.009)
#define ANG_SPEED_GAIN (-0.0004)  //Should be negative
#define ANG_TOLERANCE 0.5
#define POSITION_TOLERANCE 0.5
#define ANG_SPEED_TOLERANCE 2  //angles per second
#define POSITION_SPEED_TOLERANCE 0.001
#define MINIMUM_THRESHOLD_PWM (0.03 / MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS) //Minimum pwm to move the motor at 1 safe factor


//Maximum acceleration. Decrease if it is slipping
#define CURRENT_LIMIT (0.05 / MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS)
//Maximum breaking. Decrease if it is slipping.
#define BACKWARDS_CURRENT_LIMIT  (0.05 / MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS)

#define UPDATE_RATE_ACTUATORS_MILISECONDS 10
#endif // CONFIGFILE_H

