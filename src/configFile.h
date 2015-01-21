#ifndef CONFIGFILE_H
#define CONFIGFILE_H

//Motor definitions
#define MOTORS_OPPOSITE 1 //IF THE MOTORS ARE WIRED OPPOSITELY

/*SWAP THE MOTOR_DIRECTIONS BACK AND FORTH IF THE ROBOT IS GOING BACKWARDS
* If you swap, you also need to modify the CLOCKWISE_POSITIVE from +-1 to -+
* And also change ENCODER_OPPOSITE_MOTOR from -1 to 1;
* IF YOU MODIFY THEM, REMEMBER TO RETUNE THE MOTORCONTROL GAINS. THEY ARE SMALLER IF THE
*/
//METAL WHELL IS ON THE FRONT = 1 0 -1 1
#define MOTOR_DIRECTION_BACK 0  //Defines the value of the Dir pin when going back 1/0
#define MOTOR_DIRECTION_FRONT 1  //Defines the value of the Dir pin when going front 0/1
#define CLOCKWISE_POSITIVE 1 //Change from +1 to -1 if it is negative in the clockwise direction
#define ENCODER_OPPOSITE_MOTOR  -1 // +1 or -1 If the encoders are defined to be going opposite to the motor direction

#define RIGHT_WHEEL 1
#define RIGHT_WHEEL_PWM 1
#define RIGHT_WHEEL_DIR 9

#define LEFT_WHEEL 1
#define LEFT_WHEEL_PWM 0
#define LEFT_WHEEL_DIR 6


#define CIRCUMFERENCE_WHEEL 12.1738 // inches
#define DISTANCE_DIFFERENCE_FOR_360_DEGREES (5.472*CIRCUMFERENCE_WHEEL)

//Define values for the motors
#define MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS 0.7
#define MAXIMUM_REVOLUTIONS_PER_SECOND 3 //Useful to control the torque limiter
#define MAXIMUM_SPEED MAXIMUM_REVOLUTIONS_PER_SECOND
#define MAXIMUM_DEGREES_PER_SECOND 360
#define MAXIMUM_ANGULAR_SPEED MAXIMUM_DEGREES_PER_SECOND

//Servo definitions
#define SWIPE_TIME_MS 250
#define UPDATE_RATE_SERVOS_MILISECONDS 10
#define SORT_SERVO_PWM 15
#define ARM_SERVO_PWM 14
#define HOOK_SERVO_PWM 13
#define HOOK_START 100
#define ARM_START 7
#define SORT_START 90
//defining times for servo control in microseconds
#define TIME_FOR_HOOK 500000
#define TIME_FOR_RAISE 100000
#define TIME_FOR_SORT 100000

//SENSORS
//Ultrasonic
#define FRONT_ULTRASONIC 0
#define FRONT_ULTRASONIC_TR 0
#define FRONT_ULTRASONIC_EC 1

#define RIGHT_ULTRASONIC 0
#define RIGHT_ULTRASONIC_TR 0
#define RIGHT_ULTRASONIC_EC 0

#define LEFT_ULTRASONIC 0
#define LEFT_ULTRASONIC_TR 0
#define LEFT_ULTRASONIC_EC 0


//Encoder
#define GEAR_RATIO 29.92//30.0
#define EDGES_PER_ROTATION 32.0

#define ENC_2_WIRES 1 //If we are using two wires per encoder, and using quadrature to tell the direction
#define LEFT_ENCODER 1
#define RIGHT_ENCODER 1

#define LEFT_ENCODER_ENC_A 4 //Yellow wire or Blue
#define LEFT_ENCODER_ENC_B 5 //White wire

#define RIGHT_ENCODER_ENC_A 2 //Yellow wire or Blue
#define RIGHT_ENCODER_ENC_B 3 //White wire


//Those should be defined only if we are using only one wire per encoder
///////////////////////////////////////////
#define LEFT_ENCODER_DIR LEFT_WHEEL_DIR
#define LEFT_ENCODER_ENC 4

#define RIGHT_ENCODER_DIR RIGHT_WHEEL_DIR
#define RIGHT_ENCODER_ENC 5
///////////////////////////////////////////////////


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
#define FRONT_SHORTIR 1
#define FRONT_SHORTIR_PIN 0

#define LEFT_SHORTIR 0
#define LEFT_SHORTIR_PIN 0

#define RIGHT_SHORTIR 1
#define RIGHT_SHORTIR_PIN 1

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
//They should be increased if there is too much noise
/////////////////////////////////////
#define SHORT_IR_ALPHA 0.7
#define ULTRASHORT_IR_ALPHA 0.5
#define ULTRASONIC_ALPHA 0.7
#define ENCODER_ALPHA 0
#define ENCODER_ANGLE_ALPHA 0
#define GYROSCOPE_TOTAL_ALPHA 0.7
#define GYROSCOPE_READING_ALPHA 0.7
//////////////////////////////////////

//Sensors update Rate
#define SENSORS_UPDATE_RATE_MILISECONDS 0 //UPDATE AS OFTEN AS POSSIBLE

//Speed control
//Define gains for the angle and position control.
#define USE_GIROSCOPE_FOR_ANGLE 1
#define SPEED_CONTROL_UPDATE_RATE_MILISECONDS 50
#define FWD_ERROR_GAIN (0.30/CIRCUMFERENCE_WHEEL)    //Should be positive
#define FWD_SPEED_GAIN (-0.0/CIRCUMFERENCE_WHEEL)  //Should be negative
#define ANG_ERROR_GAIN (0.009)
#define ANG_SPEED_GAIN (-0.0004)  //Should be negative
#define ANG_TOLERANCE 0.5
#define POSITION_TOLERANCE 0.01
#define ANG_SPEED_TOLERANCE 2  //angles per second
#define POSITION_SPEED_TOLERANCE 0.001
#define MINIMUM_THRESHOLD_PWM (0.03 / MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS) //Minimum pwm to move the motor at 1 safe factor
#define MAXIMUM_DYNAMIC_TURN_ANGLE 90 //The maximum angle in which the robot can turn and move forward or back at the same time
#define MAXIMUM_DYNAMIC_TURN_ANGLE_SPEED 180

//Maximum acceleration. Decrease if it is slipping
#define CURRENT_LIMIT (0.1 / MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS)
//Maximum breaking. Decrease if it is slipping.
#define BACKWARDS_CURRENT_LIMIT  (0.05 / MAXIMUM_NORMALIZED_SAFE_SPEED_MOTORS)

#define UPDATE_RATE_ACTUATORS_MILISECONDS 5
#define UPDATE_RATE_STATE_MACHINE_MICROSECONDS 0

/////////////////////////////////////////////////////////////
//Here starts definitions related to the states of the robot.
#define WALL_FOLLOW_CARROT_DISTANCE_INCHES 12
#define WALL_FOLLOW_WALL_DISTANCE_INCHES 17
#define WALL_FOLLOW_MAXIMUM_WALL_DISTANCE_INCHES 35

#define BLOCK_COLLECT_RESET_TIME_MS 300
#define BLOCK_COLLECT_GRAB_TIME_MS 200
#define BLOCK_COLLECT_LIFT_TIME_MS 400
#define BLOCK_COLLECT_SORT_TIME_MS 200
#define BLOCK_COLLECT_RELEASE_TIME_MS 200
#define BLOCK_COLLECT_SWIPE_TIME_MS 900
#define BLOCK_COLLECT_MAX_TIME_MOVING 400
#define BLOCK_COLLECT_DISTANCE_MOVE 6


/////////////////////////////
//Servos control

#define PI 3.14159
#endif // CONFIGFILE_H

