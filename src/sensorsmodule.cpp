#include "sensorsmodule.h"
sensorsModule::sensorsModule():
    //Here is the initializer. It defines initial values for all the variables and initializes all the objects
    frontUltrasonicData(0),
    rightUltrasonicData(0),
    leftUltrasonicData(0),
    backUltrasonicData(0),

    rightEncoderMovement(0),
    leftEncoderMovement(0),
    encoderAngle(0),

    frontUltraShortIRData(0),
    rightUltraShortIRData(0),
    leftUltraShortIRData(0),
    backUltraShortIRData(0),

    frontShortIRData(0),
    rightShortIRData(0),
    leftShortIRData(0),
    backShortIRData(0),
    colorSensorData(0),
    teamData(0),
    onData(0),
    gyroscopeAngle(0),
    gyroscopeReading(0),
    ultrasonicAlpha(ULTRASONIC_ALPHA),
    ultraShortIRAlpha(ULTRASHORT_IR_ALPHA),
    shortIRAlpha(SHORT_IR_ALPHA),
    colorSensorAlpha(0),
    teamDataAlpha(0),
    onDataAlpha(0),
    encoderAlpha(ENCODER_ALPHA),
    encoderAngleAlpha(ENCODER_ANGLE_ALPHA),
    gyroscopeTotalAlpha(GYROSCOPE_TOTAL_ALPHA),
    gyroscopeReadingAlpha(GYROSCOPE_READING_ALPHA)

    #if FRONT_ULTRASONIC
    ,frontUltrasonic(FRONT_ULTRASONIC_TR,FRONT_ULTRASONIC_EC)
    #endif

    #if RIGHT_ULTRASONIC
    ,rightUltrasonic(RIGHT_ULTRASONIC_TR,RIGHT_ULTRASONIC_EC)
    #endif

    #if LEFT_ULTRASONIC
    ,leftUltrasonic(LEFT_ULTRASONIC_TR,LEFT_ULTRASONIC_EC)
    #endif

    #if BACK_ULTRASONIC
    ,backUltrasonic(BACK_ULTRASONIC_TR, BACK_ULTRASONIC_EC)
    #endif
    #if ENC_2_WIRES
    #if RIGHT_ENCODER
    ,rightEncoder(RIGHT_ENCODER_ENC_A,RIGHT_ENCODER_ENC_B,0)
    #endif

    #if LEFT_ENCODER
    ,leftEncoder(LEFT_ENCODER_ENC_A,LEFT_ENCODER_ENC_B,1)
    #endif
    #else //ENC_2_WIRES
    #if RIGHT_ENCODER
    ,rightEncoder(RIGHT_ENCODER_ENC)
    #endif

    #if LEFT_ENCODER
    ,leftEncoder(LEFT_ENCODER_ENC)
    #endif
    #endif//ENC_2_WIRES


    #if FRONT_ULTRASHORTIR
    ,frontUltraShortIR(FRONT_ULTRASHORTIR_PIN)
    #endif

    #if BACK_ULTRASHORTIR
    ,backUltraShortIR(BACK_ULTRASHORTIR_PIN)
    #endif

    #if RIGHT_ULTRASHORTIR
    ,rightUltraShortIR(RIGHT_ULTRASHORTIR_PIN)
    #endif

    #if LEFT_ULTRASHORTIR
    ,leftUltraShortIR(RIGHT_ULTRASHORTIR_PIN)

    #endif



    #if FRONT_SHORTIR
    ,frontShortIR(FRONT_SHORTIR_PIN)
    #endif

    #if BACK_SHORTIR
    ,backShortIR(BACK_SHORTIR_PIN)
    #endif

    #if RIGHT_SHORTIR
    ,rightShortIR(RIGHT_SHORTIR_PIN)
    #endif

    #if LEFT_SHORTIR
    ,leftShortIR(LEFT_SHORTIR_PIN)
    #endif

    #if GYROSCOPE
    ,mygyroscope(GYROSCOPE_CHIP_PIN, GYROSCOPE_SPI_PIN)
    #endif
//
    #if COLOR_DETECTOR
    ,colorSensor(COLOR_DETECTOR_PIN)
    #endif

    #if TEAM_BUTTON
    ,teamButton(TEAM_PIN)
    #endif

  #if ON_BUTTON
  ,onButton(ON_PIN)
  #endif

{
    running=1;
    runThread = new std::thread(run,this);
}


sensorsModule::~sensorsModule(){
    running=0;
    runThread->join();
    delete runThread;
}


void sensorsModule::run(sensorsModule * sensors){
    int started =0;

    while (sensors->running){
        #if FRONT_ULTRASONIC
        updateSensor(&sensors->frontUltrasonic,&sensors->frontUltrasonicData, sensors->ultrasonicAlpha,started);
        #endif

        #if RIGHT_ULTRASONIC
        updateSensor(&sensors->rightUltrasonic,&sensors->rightUltrasonicData, sensors->ultrasonicAlpha,started);
        #endif

        #if LEFT_ULTRASONIC
        updateSensor(&sensors->leftUltrasonic,&sensors->leftUltrasonicData, sensors->ultrasonicAlpha,started);
        #endif

        #if BACK_ULTRASONIC
        updateSensor(&sensors->backUltrasonic,&sensors->backUltrasonicData, sensors->ultrasonicAlpha,started);
        #endif

        #if RIGHT_ENCODER
        updateSensor(&sensors->rightEncoder,&sensors->rightEncoderMovement, sensors->encoderAlpha,started);
        #endif

        #if LEFT_ENCODER
        updateSensor(&sensors->leftEncoder,&sensors->leftEncoderMovement, sensors->encoderAlpha,started);
        #endif
        #if RIGHT_ENCODER*LEFT_ENCODER
        double newEncoderAngle= (-sensors->rightEncoderMovement+sensors->leftEncoderMovement)/DISTANCE_DIFFERENCE_FOR_360_DEGREES*360;
        updateData(&sensors->encoderAngle,newEncoderAngle, sensors->encoderAngleAlpha,started);
        #endif



        #if FRONT_ULTRASHORTIR
        updateSensor(&sensors->frontUltraShortIR,&sensors->frontUltraShortIRData, sensors->ultraShortIRAlpha,started);
        #endif

        #if BACK_ULTRASHORTIR
        updateSensor(&sensors->backUltraShortIR,&sensors->backUltraShortIRData, sensors->ultraShortIRAlpha,started);
        #endif

        #if RIGHT_ULTRASHORTIR
        updateSensor(&sensors->backUltraShortIR,&sensors->backUltraShortIRData, sensors->ultraShortIRAlpha,started);
        #endif

        #if LEFT_ULTRASHORTIR
        updateSensor(&sensors->leftUltraShortIR,&sensors->leftUltraShortIRData, sensors->ultraShortIRAlpha,started);
        #endif



        #if FRONT_SHORTIR
        updateSensor(&sensors->frontShortIR,&sensors->frontShortIRData, sensors->shortIRAlpha,started);
        #endif

        #if BACK_SHORTIR
        updateSensor(&sensors->backShortIR,&sensors->backShortIRData, sensors->backIRAlpha,started);
        #endif

        #if RIGHT_SHORTIR
        updateSensor(&sensors->rightShortIR,&sensors->rightShortIRData, sensors->shortIRAlpha,started);
        #endif

        #if LEFT_SHORTIR
        updateSensor(&sensors->leftShortIR,&sensors->leftShortIRData, sensors->shortIRAlpha,started);
        #endif

        #if GYROSCOPE
        updateData(&sensors->gyroscopeAngle, sensors->mygyroscope.getTotal(), sensors->gyroscopeTotalAlpha,started);
        updateData(&sensors->gyroscopeReading, sensors->mygyroscope.getReading(), sensors->gyroscopeReadingAlpha,started);
        #endif

        #if COLOR_DETECTOR
        updateSensor(&sensors->colorSensor,&sensors->colorSensorData, sensors->colorSensorAlpha,started);
        #endif

        #if TEAM_BUTTON
        updateSensor(&sensors->teamButton,&sensors->teamData, sensors->teamDataAlpha,started);
        #endif

        #if ON_BUTTON
        updateSensor(&sensors->onButton,&sensors->onData, sensors->onDataAlpha,started);
        #endif

        updateTime(sensors);
        usleep(SENSORS_UPDATE_RATE_MILISECONDS);
        started=1;
    }

}

void sensorsModule::updateSensor(sensorsSuperClass *sensor, volatile double *data, float alpha, int started){
    //We want to have a time out here on the getData
    double newData = sensor->getData();
    updateData(data,newData,alpha,started);
}

void sensorsModule::updateData(volatile double *previousData, double newData, float alpha, int started){
    if (started){
        if (isinf(*previousData)){
            *previousData=newData;
        }
        else{
        *previousData = kalmanFilter(*previousData,newData, alpha);
        }
    }
    else{
        *previousData=newData;
    }
}

double sensorsModule::kalmanFilter(double previousData, double newData, float alpha){
    return (previousData*alpha + newData * (1-alpha));
}


void sensorsModule::updateTime(sensorsModule *sensors){
    sensors->timeMicrosecondsSinceEpoch = std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::system_clock::now().time_since_epoch()).count(); //magic from Stack Overflow
}

double sensorsModule::getAngle(){
    if(USE_GIROSCOPE_FOR_ANGLE){
        return gyroscopeAngle;
    }
    else{
        return encoderAngle;
    }
}

double sensorsModule::getPosition(){
    return (rightEncoderMovement+leftEncoderMovement)/2;
}
